
#include "mem.h"
#include "mem_os.h"
#include "common.h"
#include <stdio.h>

struct fb{ 
	size_t size;
	struct fb *next;
};


//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
void mem_init() {
	*((struct fb*)memory) = (struct fb*) ( (char *) get_memory_adr() + sizeof(struct fb*) );
	struct fb* zoneLibre=*((struct fb*)memory);
	zoneLibre->size=get_memory_size()-sizeof(struct fb*);
	zoneLibre->next=null;
	mem_fit(mem_fit_first);
}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
void* mem_alloc(size_t size) {
   /* A COMPLETER */ 
	
        return NULL;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
void mem_free(void* zone) {
   /* A COMPLETER */ 
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
// mem_show
//-------------------------------------------------------------
void mem_show(void (*print)(void *, size_t, int free)) {
   /* A COMPLETER */ 
}

//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_fit(mem_fit_function_t* mff) {
   /* A COMPLETER */ 
}

//-------------------------------------------------------------
// Stratégies d'allocation 
//-------------------------------------------------------------
struct fb* mem_first_fit(struct fb* head, size_t size) {
   /* A COMPLETER */
    if(head==null) return NULL;
    struct fb* p=head;
    
    do{
	if (p->next->size >= size ) {
	//il y a la place de stoquer notre donnée
		if(p->next->size - size >= sizeof(struct fb*) ){ 
		//si il y a la place pour créer une nouvelle zone libre à la suite de la zone à alouer
			void* adr_aloue = p->next;
			struct fb* zone_libre=p->next+size;//on créer la nouvelle zone libre à la suite de ce qui va être donnée à l'utilisateur
			zone_libre->size =p->next->size - size ;
			zone_libre->next = p->next->next;
			p->next=zone_libre;
			return adr_aloue;
		}
		else {
		//si il n'y a pas de place on prend toute sa zone libre
			void* adr_aloue = p->next;
			p->next=p->next->next;
			return adr_aloue;
		}
	}
	else{
		p=p->next; // on passe a la zone libre suivante car pas de place
	}
    }while(p->next!=NULL);
    return NULL;
}
//-------------------------------------------------------------
struct fb* mem_best_fit(struct fb* head, size_t size) {
   /* A COMPLETER */ 
   if(head==null) return NULL;
    struct fb* p=head;
    void* adr_avant = p;
    void* adr_aloue = p->next;
	
    do{
	if (adr_aloue->size >= p->next->size && p->next->size >= size ) {
	//il y a la place de stoquer notre donnée et si la place et plus petit que ce qu'on a trouvé avant
		adr_avant = p; //On prend l'adr d'avant pour continuer la chaine
		adr_aloue = p->next; //On prend la nouveau zone libre jusque la prochaine 
		p=p->next;//On avance
	}
	else{
		p=p->next; // on passe a la zone libre suivante car pas de place
	}
    }while(p->next!=NULL);
    if(adr_aloue->size - size >= sizeof(struct fb*) ){ 
	//si il y a la place pour créer une nouvelle zone libre à la suite de la zone à alouer
			
		struct fb* zone_libre=adr_aloue+size;//on créer la nouvelle zone libre à la suite de ce qui va être donnée à l'utilisateur
		zone_libre->size =adr_aloue->size - size ;
		zone_libre->next = adr_aloue->next;
		adr_avant->next=zone_libre;
		return adr_aloue;
    }
    else {
    //si il n'y a pas de place on prend toute sa zone libre
		adr_avant->next=adr_aloue->next;
		return adr_aloue;
    }
    return NULL;

}
//-------------------------------------------------------------
struct fb* mem_worst_fit(struct fb* head, size_t size) {
   /* A COMPLETER */ 
    if(head==null) return NULL;
    struct fb* p=head;
    void* adr_avant = p;
    void* adr_aloue = p->next;
	
    do{
	if (adr_aloue->size <= p->next->size && p->next->size >= size ) {
	//il y a la place de stoquer notre donnée et si la place et plus grand que ce qu'on a trouvé avant
		adr_avant = p; //On prend l'adr d'avant pour continuer la chaine
		adr_aloue = p->next; //On prend la nouveau zone libre jusque la prochaine 
		p=p->next;//On avance
	}
	else{
		p=p->next; // on passe a la zone libre suivante car pas de place
	}
    }while(p->next!=NULL);
    if(adr_aloue->size - size >= sizeof(struct fb*) ){ 
	//si il y a la place pour créer une nouvelle zone libre à la suite de la zone à alouer
			
		struct fb* zone_libre=adr_aloue+size;//on créer la nouvelle zone libre à la suite de ce qui va être donnée à l'utilisateur
		zone_libre->size =adr_aloue->size - size ;
		zone_libre->next = adr_aloue->next;
		adr_avant->next=zone_libre;
		return adr_aloue;
    }
    else {
    //si il n'y a pas de place on prend toute sa zone libre
		adr_avant->next=adr_aloue->next;
		return adr_aloue;
    }
    return NULL;
}
