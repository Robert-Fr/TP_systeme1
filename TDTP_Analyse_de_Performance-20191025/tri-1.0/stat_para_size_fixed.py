#arguments : <size> <nom_fichier_min> <nb_proc>
#ouverture du fichier contenant les temps minimum pour une taille donnee :
import sys
import os
import subprocess
def calcul_stats_tri_size_fixed(size="5",nb_proc="8") :
    min_time=[]
    min_time_file=open("min_time_seq.txt","r")
    it=0
    #remplissage de la liste avec les valeurs du fichier
    for line in min_time_file:
        min_time.append(float(line))
        it+=1
    min_time_file.close()

    acc_file_name="acc_size_fixed"+ size + ".txt"
    eff_file_name="eff_size_fixed"+ size + ".txt"
    #ouverture en ecriture des deux fichiers et ecriture de l'entete
    acc_file=open(acc_file_name,"w")
    acc_file.write("NB_Thread;Run_ID;Acc"+"\n")
    eff_file=open(eff_file_name,"w")
    eff_file.write("NB_Thread;Run_ID;Eff"+"\n")

    nom_fichier_sortie="test_"+size+".txt"

    for i in [2,4,8,16,32]:
        #generer un vecteur avec la nouvelle taille
        for j in range(100):
            #execution de ./tri_thread sys.argv[1] size NPO utiliser le nouveau vecteur 
            out=subprocess.check_output("./tri_threads --parallelism " + str(i) + " --rusage < " +nom_fichier_sortie,shell=True)
            out.decode("utf-8")
            #recuperer la valeur de retour 
            acc=min_time[int(size)-5]/float(out)
            eff=acc/float(nb_proc)
            #ecriture dans fich_acc -> 10^i;j;acc
            acc_file.write(str(i)+";"+str(j)+";"+str(acc)+"\n")
            #ecriture dans fich_eff -> 10^i;j;eff
            eff_file.write(str(i)+";"+str(j)+";"+str(eff)+"\n")

    acc_file.close()
    eff_file.close()
    print("execution taille fixe pour taille = " + + " terminee")
