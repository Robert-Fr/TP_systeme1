#arguments : <size> <nom_fichier_min> <nb_proc>
#ouverture du fichier contenant les temps minimum pour une taille donnée :
import sys
import os

min_time=[]
min_time_file=open(sys.argv[2],"r")
it=0
#remplissage de la liste avec les valeurs du fichier
for line in min_time_file:
    min_time.append(float(line))
    it+=1
min_time_file.close()

acc_file_name="acc_size_fixed"+ sys.argv[1] + ".txt"
eff_file_name="eff_size_fixed"+ sys.argv[1] + ".txt"
#ouverture en écriture des deux fichiers et ecriture de l'entete
acc_file=open(acc_file_name,"w")
acc_file.write("NB_Thread;Run_ID;Acc"+"\n")
eff_file=open(eff_file_name,"w")
eff_file.write("NB_Thread;Run_ID;Eff"+"\n")

nom_fichier_sortie="test_"+sys.argv[1]+".txt"

for i in [2,4,8,16,32]:
    #generer un vecteur avec la nouvelle taille
    for j in range(100):
        #execution de ./tri_thread sys.argv[1] size NPO utiliser le nouveau vecteur 
        out=subprocess.check_output("./tri_threads --parallelism " + str(i) + " --rusage < " +nom_fichier_sortie,shell=True)
        out.decode("utf-8")
        #recupérer la valeur de retour 
        acc=min_time[sys.argv[1]-5]/float(out)
        eff=acc/float(sys.argv[3])
        #ecriture dans fich_acc -> 10^i;j;acc
        acc_file.write(str(i)+";"+str(j)+";"+str(acc)+"\n")
        #ecriture dans fich_eff -> 10^i;j;eff
        eff_file.write(str(i)+";"+str(j)+";"+str(eff)+"\n")

acc_file.close()
eff_file.close()
print("execution terminee")
