#include "Fichier.h

FICHIER *ouvrir(char* nom,char mode){
    //entrée incorrecte
    if(nom==NULL || (mode != "L" && mode!="E"))
        return NULL;
    //allocation des espaces mémoire necessaire
    Buffer* new_buff=malloc(sizeof(Buffer));
    char* s=malloc(BUFFER_SIZE * sizeof(char));
    new_buff->b=s;
    //initialisation
    new_buff->start_index=0;
    new_buff->curr_index=0;
    //ouverture en lecture ou ecriture
    int file_desc;
    if(mode=="L")
        file_desc=open(nom,O_RDONLY);
    else
        file_desc=open(nom,O_WRONLY);
    //création de la structure finale
    FICHIER * f=malloc(sizeof(FICHIER));
    f->buff=new_buff;
    f->mode=mode;
    f->f=file_desc;
    
    return f;
}
int fermer (FICHIER* f){
    //NPO il faut finir d'écrire ce qu'il y a dans le buffer s'il n'est pas vide
    free(f->buff);
    close(f->f);
    free(f);
    return 0;
}
int lire(void* p , unsigned int taille , unsigned int nbelem , FICHIER* f){
    
}
int ecrire(void* p , unsigned int taille , unsigned int nbelem , FICHIER* f){
    int nb_octets_ecrit=0;
    void* p_curr=p;
    int curr_data_byte=0;
    //différent cas possible :
    if(f->mode!="E")
        return 0;
    //les éléments sont trop grands pour rentrer dans le buffer :
    if(taille > BUFFER_SIZE){
        return nb_octets_ecrit;
    }
    else {
        //on ecrit tant que la chaine donnée en parametre n'a pas été écrite en entier
        while(p_curr!= p+taille*nbelem){
            //on ecrit un maximum dans le buffer avant de faire un appel a write
            while(f->buff->curr_index<BUFFER_SIZE-taille){
                //on copie octet par octet l'element courrant
                curr_data_byte=0;
                while(curr_data_byte<taille){
                    f->buff->b[f->buff->curr_index+curr_data_byte]=p_curr[curr_data_byte];
                    curr_data_byte++;
                }
                p_curr=p_curr+taille;
                f->buff->curr_index=f->buff->curr_index+taille;
            }
            //le buffer est plein on ecrit le contenu dans le fichier puis remise a 0 du buffer
            nb_octets_ecrit+=f->buff->curr_index- f->buff->start_index;
            write(f->f,f->buff,f->buff->curr_index- f->buff->start_index);
            f->buff->curr_index=0;
        }
        
    }
}