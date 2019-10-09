
#include "mem.h"
#include "mem_os.h"
#include "common.h"
#include <stdio.h>
#define MEM_ALIGN 8

//char* memory[MEMORY_SIZE]; 
//structure représentant les informations d'un "free block"
struct fb{ 
	size_t size;
	struct fb *next;
};
//structure représentant les informations d'un "allocated block"
struct ab{
    size_t size;// NOTA BENE : size = la taille de la zone ET la sizeof(struct ab) pour soulager la synthaxe
};
struct tete_memoire {
	struct fb* head;
	mem_fit_function_t* fit_func;
};

//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
void mem_init() {
	//struct fb* adr_fin_en_tete=(((struct fb*) memory)+sizeof(struct tete_memoire* ));//car on va stoquer une en tete au debut de la mémoire
	struct tete_memoire* en_tete = get_memory_adr();
	//on gèrera l'alignement de la façon suivante:
	//on prend comme prédicat de départ avant toute allocation que l'adresse de début est une adress correctement allginé
	//on s'occupe donc uniquement d'aligné la fin de chaque zone aloué selo la taille que l'utilisateur donne
	//on rempli les champs de l'en tete :
    // en_tete->fit_func sera rempli dans mem_fit :
	mem_fit(mem_first_fit);
    //on initialise la premiere zone libre
	struct fb* zoneLibre=get_memory_adr()+sizeof(struct tete_memoire);//on place notre premiere zone libre
	zoneLibre->size=get_memory_size()-sizeof(struct tete_memoire);// on rempli la zone mémoire qui indique la taille de la zone libre
	zoneLibre->next=NULL; //on rempli la zone mémoire qui indique la prochaine zone libre
    //Notre tete de memoire pointe sur la premiere zone libre
	en_tete->head= zoneLibre;
	
}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
void* mem_alloc(size_t size) {
	void* adr_aloue;
	mem_fit_function_t* fit_fct = ((struct tete_memoire*)get_memory_adr())->fit_func; // le pointeur de fonction
	//on appelle la fonction associé à la stratégie que l'on veut appliquer pour allouer de la mémoire
	if(*fit_fct == mem_first_fit ) {
		adr_aloue=mem_first_fit((((struct tete_memoire*) get_memory_adr())->head),size);
	}
	else if (*fit_fct == mem_worst_fit ) {
		adr_aloue=mem_worst_fit((((struct tete_memoire*) get_memory_adr())->head),size);
	}
	else if (*fit_fct == mem_best_fit ) {
		adr_aloue=mem_best_fit((((struct tete_memoire*) get_memory_adr())->head),size);
	}	
        return adr_aloue;
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
	((struct tete_memoire *) get_memory_adr()) -> fit_func= mff;
}

//-------------------------------------------------------------
// Stratégies d'allocation 
//-------------------------------------------------------------
struct fb* mem_first_fit(struct fb* head, size_t size) {
    if(head==NULL) return NULL;
    struct fb* p=head;//notre block de parcours
    struct fb* p_pred=head;//notre block de parcours qui précède le block défini si dessus
    //on gère l'alignement
    if (size % MEM_ALIGN != 0)
        size+=(MEM_ALIGN - (size % MEM_ALIGN));
    
    while(p != NULL) {
        if (p->size >= size + sizeof(struct ab)) {
        //il y a la place de stoquer notre donnée + la structure qui donne la taille de la zone à occuper
            if(p->size - (size+sizeof(struct ab)) >= sizeof(struct fb) ){ 
            //si il y a la place pour créer une nouvelle zone libre à la suite de la zone à alouer
                void* adr_aloue = p;
                //on sauvegarde ce qui se trouve dans p car va être écrasé
                size_t taille_zone=p->size;
                struct fb* suivant=p->next;
                //on place au début de cette zone aloué un struct ab pour pouvoir récupérer sa taille si besoin
                struct ab* new_alloc_block=(struct ab*)p;
                new_alloc_block->size=size+sizeof(struct ab);
                //on gère l'allignement 
                if (new_alloc_block->size % MEM_ALIGN != 0)
                    new_alloc_block->size+=(MEM_ALIGN - (new_alloc_block->size % MEM_ALIGN));
                struct fb* zone_libre=(struct fb *)((char*)p+(new_alloc_block->size));//on créer la nouvelle zone libre à la suite de ce qui va être donnée à l'utilisateur
                zone_libre->size =taille_zone -  new_alloc_block->size ;
                zone_libre->next = suivant;
                p_pred->next=zone_libre;//le block qui précède notre block de parcours va désormais pointer vers la nouvelle zone libre
                return adr_aloue;
            }
            else {
            //si il n'y a pas de place on prend toute sa zone libre
                void* adr_aloue = p;
                p_pred->next=p->next;
                //on place au début de cette zone aloué un struct ab pour pouvoir récupérer sa taille si besoin
                 struct ab* new_alloc_block=(struct ab*)p;
                new_alloc_block->size=size+sizeof(struct ab);
                //on gère l'allignement 
                if (new_alloc_block->size % MEM_ALIGN != 0)
                    new_alloc_block->size+=(MEM_ALIGN - (new_alloc_block->size % MEM_ALIGN));
                return adr_aloue;
            }
        }
        else{
            p_pred=p;
            p=p->next; // on passe a la zone libre suivante car pas de place
        }
    }
    return NULL;
}
//-------------------------------------------------------------
struct fb* mem_best_fit(struct fb* head, size_t size) {
   if(head==NULL) return NULL;
    struct fb* p=head;
    struct fb* avant = p;
    struct fb* aloue = p->next;
    void* adr_aloue = p->next;
	
    do{
	if (aloue->size >= p->next->size && p->next->size >= size ) {
	//il y a la place de stoquer notre donnée et si la place et plus petit que ce qu'on a trouvé avant
		avant = p; //On prend l'adr d'avant pour continuer la chaine
		aloue = p->next; //On prend la nouveau zone libre jusque la prochaine
		adr_aloue = p->next;
 
		p=p->next;//On avance
	}
	else{
		p=p->next; // on passe a la zone libre suivante car pas de place
	}
    }while(p->next!=NULL);
    if(aloue->size - size >= sizeof(struct fb*) ){ 
	//si il y a la place pour créer une nouvelle zone libre à la suite de la zone à alouer
			
		struct fb* zone_libre=aloue+size;//on créer la nouvelle zone libre à la suite de ce qui va être donnée à l'utilisateur
		zone_libre->size =aloue->size - size ;
		zone_libre->next = aloue->next;
		avant->next=zone_libre;
		return adr_aloue;
    }
    else {
    //si il n'y a pas de place on prend toute sa zone libre
		avant->next=aloue->next;
		return adr_aloue;
    }
    return NULL;

}
//-------------------------------------------------------------
struct fb* mem_worst_fit(struct fb* head, size_t size) {
    if(head==NULL) return NULL;
    struct fb* p=head;
    struct fb* avant = p;
    struct fb* aloue = p->next;
    void* adr_aloue = p->next;
	
    do{
	if (aloue->size <= p->next->size && p->next->size >= size ) {
	//il y a la place de stoquer notre donnée et si la place et plus grand que ce qu'on a trouvé avant
		avant = p; //On prend l'adr d'avant pour continuer la chaine
		aloue = p->next; //On prend la nouveau zone libre jusque la prochaine 
		adr_aloue = p->next;
		p=p->next;//On avance
	}
	else{
		p=p->next; // on passe a la zone libre suivante car pas de place
	}
    }while(p->next!=NULL);
    if(aloue->size - size >= sizeof(struct fb*) ){ 
	//si il y a la place pour créer une nouvelle zone libre à la suite de la zone à alouer
			
		struct fb* zone_libre=aloue+size;//on créer la nouvelle zone libre à la suite de ce qui va être donnée à l'utilisateur
		zone_libre->size =aloue->size - size ;
		zone_libre->next = aloue->next;
		avant->next=zone_libre;
		return adr_aloue;
    }
    else {
    //si il n'y a pas de place on prend toute sa zone libre
		avant->next=aloue->next;
		return adr_aloue;
    }
    return NULL;
}
