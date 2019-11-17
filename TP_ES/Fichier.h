#ifndef _FICHIER_H_
#define _FICHIER_H_
#include<unistd.h>

#define BUFFER_SIZE 512

typedef struct Buffer{
    char *b;
    int start_index;
    int curr_index;
}

typedef struct FICHIER{
    int f;
    char mode; // L -> lecture | E -> ecriture
    Buffer *buff;
}

FICHIER *ouvrir(char* nom,char mode);
int fermer (FICHIER* f);
int lire(void* p , unsigned int taille , unsigned int nbelem , FICHIER* f);
int ecrire(void* p , unsigned int taille , unsigned int nbelem , FICHIER* f);

#endif