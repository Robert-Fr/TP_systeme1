#!/bin/bash
taille_thread=( '2' '4' '8' '16' '32' )

for $nb_th in taille_thread 
do
    #creation nom du fichier de sortie de format tmp_moyen_para_NBTH.txt
    nom_fichier_sortie="tmp_moyen_para_" + $nb_th + ".txt"
    let "moyenne_exec_para = 0"
    let "i = 0"
    while [ $i -le 99 ]
    do
        let "moyenne_exec_para = moyenne_exec_para + ./tri_thread $nb_th < fich_vecteur_taille.txt"
        let"i = i+1"
    done
done