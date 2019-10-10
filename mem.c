
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
    size_t size;// NOTA BENE : size = la taille de la zone SANS la sizeof(struct ab)  qui se trouve avant l'adresse d'une zone occupée
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
	zoneLibre->size=get_memory_size()-sizeof(struct tete_memoire)-sizeof(struct fb);// on rempli la zone mémoire qui indique la taille de la zone libre
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
    //NOTA BENE : on ne fait pas d verification sur le parametre pour le moment 
    // c'est a dire qu'on ne verifie pas que c'est bien une adresse dans notre mémoire, qui désigne bien une zone occupée, ...
   //on récupère la liste des zones libres pour parcourir la mémoire
   struct fb* p=((struct tete_memoire*)get_memory_adr())->head;
   struct fb* p_pred=((struct tete_memoire*)get_memory_adr())->head;
   struct ab* to_free_bloc= (struct ab*)zone;
   //on gere le cas particulier ou la memoire est pleine
   
   //on gere le cas particulier ou la zone à libérer est avant la tete de la liste des zones libres 
   
   while(p!=NULL){
        //si on a pas encore dépassé la zone à libérer
        if((char*)p <= (char*)to_free_bloc){
            //on avance dans notre parcours
            p_pred=p;
            p=p->next;
        }
        //si on à dépassé la zone à libérer on peut procéder à la libération
        else {
            //on récupère la taille de la zone à libérer :
            size_t to_free_size=(to_free_bloc->size) + sizeof(struct ab);
            //on initialise un pointeur sur zone libre, on le remplira en fonction du cas dans lequel on se trouve 
            struct fb* new_free_bloc;
            //Est ce que le bloc à libérer est collé au prédécesseur de notre bloc de parcours ?
            if( ((char*)p_pred)+p_pred->size+sizeof(struct fb) == ((char* )to_free_bloc)-sizeof(struct ab)){
                //si c'est le cas : la nouvelle zone libre commence à partir du prédécesseur et s'étend (au moins) jusqu'a la fin du bloc à libérer
                new_free_bloc=p_pred;
                new_free_bloc->size= p_pred->size + to_free_size;
            }
            else{
                //sinon : la nouvelle zone libre commence à partir de l'adresse donné -sizeof(la structure qui décrit un bloc occupé) et s'étend (au moins) jusqu'a la fin du bloc à libérer
                new_free_bloc=(struct fb*) (((char*)to_free_bloc) -sizeof(struct ab));
                new_free_bloc->size=to_free_size;
                p_pred->next=new_free_bloc;
            }
            //est ce que le bloc à libérer est collé à notre bloc de parcours ?
            if( (struct fb*)(((char* )to_free_bloc)+(to_free_bloc->size))== p ){
                // si c'est le cas la nouvelle zone libre s'étend jusqu'a la fin de notre bloc de parcours
                //on modifie donc la taille et le suivant :
                new_free_bloc->size += p->size +sizeof(struct fb);
                new_free_bloc->next = p->next;
            }
            else{
                //sinon : la nouvelle zone libre s'étend jusqu'a la fin de la zone à libérer
                //on ne modifie pas la taille, mais on dit qui est le suivant :
                new_free_bloc->next = p;
            }
            //on a finit la libération
            return;
        }
   }
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
        if (p->size+sizeof(struct fb) >= size + sizeof(struct ab)) {
        //il y a la place de stoquer notre donnée + la structure qui donne la taille de la zone à occuper
            if(p->size + sizeof(struct fb) - (size+sizeof(struct ab)) >= sizeof(struct fb) ){ 
            //si il y a la place pour créer une nouvelle zone libre à la suite de la zone à alouer
                void* adr_aloue = ((char*)p)+sizeof(struct ab);
                //on sauvegarde ce qui se trouve dans p car va être écrasé
                size_t taille_zone=p->size+sizeof(struct fb);
                struct fb* suivant=p->next;
                //on place au début de cette zone aloué un struct ab pour pouvoir récupérer sa taille si besoin
                struct ab* new_alloc_block=(struct ab*)p;
                new_alloc_block->size=size;//+sizeof(struct ab);
                //on gère l'allignement 
                if (new_alloc_block->size % MEM_ALIGN != 0)
                    new_alloc_block->size+=(MEM_ALIGN - (new_alloc_block->size % MEM_ALIGN));
                //on créer la nouvelle zone libre à la suite de ce qui va être donnée à l'utilisateur
                struct fb* zone_libre=(struct fb *)((char*)p+(new_alloc_block->size )+sizeof(struct ab));
                zone_libre->size =taille_zone -  new_alloc_block->size - sizeof(struct ab) -sizeof(struct fb) ;
                zone_libre->next = suivant;
                p_pred->next=zone_libre;//le block qui précède notre block de parcours va désormais pointer vers la nouvelle zone libre
                return adr_aloue;
            }
            else {
            //si il n'y a pas de place on prend toute sa zone libre
                void* adr_aloue = ((char*)p)+sizeof(struct ab);
                p_pred->next=p->next;
                //on place au début de cette zone aloué un struct ab pour pouvoir récupérer sa taille si besoin
                 struct ab* new_alloc_block=(struct ab*)p;
                new_alloc_block->size=size;//+sizeof(struct ab);
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
