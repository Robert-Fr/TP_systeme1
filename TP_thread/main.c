#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct args{
	int deb;//debut de la zone de recherche : indice entre 0 et size (tab)-2
	int fin;//fin de la zone de recherche : indice entre 1 et size (tab)-1
	int *trouver;
	int nb_a_trouver;//le numéro recherché dans le tableau
	int *tab; //le tableau que les th vont se partager
}args;

void  *search(void* p){
        args* param=(args*)p;
	int tid=pthread_self();
	
        int i=param->deb;
        while((i<param->fin) && (*(param->trouver)==0)){
        //on écrit l'id du thread pour indiquer aux autres que l'on à trouvé : il faut arreter de chercher
                if((param->tab)[i] == param->nb_a_trouver ){
                        (*(param->trouver))=tid;
                        printf("je suis le thread numero %d et j'ai trouvé le numéro recherché\n",tid);
                }
                i++;
        }
        if((*(param->trouver)!=tid ))
                 printf("je suis le thread numero %d et je n'ai pas trouvé le numéro recherché\n",tid);
}


int main(int argc,char* argv[argc]){
	if (argc != 4){
		printf("erreur d'usage :\n%s <taille_tab> <nb_threads> <nombre_a_chercher>\n",argv[0]);
		exit(-1);
	}
	//on récupère les arguments de la ligne de commande :
	int size=atoi(argv[1]);
	int nb_th=atoi(argv[2]);
	int nb_a_trouver=atoi(argv[3]);
	printf("%d %d %d \n",size,nb_th,nb_a_trouver);
	int* tab= malloc(size*sizeof(int));
	int *trouver=malloc(sizeof(int));//On modifiera cette valeur si le nombre est trouvé dans le tableau
	*trouver =0;
	// on declare le tableau de threads
	pthread_t *tids=malloc(nb_th*sizeof(pthread_t));
	int i;
	//on rempli le tableau d'entier :
	//printf("Veuillez saisir %d entiers svp\n",size);
	for(i=0;i<size;i++){
	       // scanf("%d",&tab[i]);
	       tab[i]=i;
	}
	for(i=0;i<nb_th;i++){
	        //on déclare les parametre qu'on va passer à notre thread
	        args *parameters=malloc(sizeof(args));
	        (parameters->deb)=i*(size/nb_th);
	        if(i==nb_th-1)//si je suis le dernier thread je prend jusqu'a la fin
	                parameters->fin=size;
	        else
	                parameters->fin=(i+1)*(size/nb_th);
	        parameters->trouver=trouver;
	        parameters->nb_a_trouver=nb_a_trouver;
	        parameters->tab=tab;
	        printf("parametre du thread numéro %d :\ndebut : %d\nfin : %d\n",i, parameters->deb, parameters->fin);
	        //on lance le thread
	        pthread_create(&tids[i],NULL,search,parameters);
	}
	//on attend la fin de tout les threads:
	for(i=0;i<nb_th;i++){
	        pthread_join(tids[i],NULL);
	}
	//on affiche si le nb a été trouvé:
	printf("%d à été trouvé par le thread numéro %d\n",nb_a_trouver,*trouver);
	free(tids);
	free(tab);
	return EXIT_SUCCESS;
}
