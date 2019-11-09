#arguments : <nb_threads> <nom_fichier_min> <nb_proc>
#ouverture du fichier contenant les temps minimum pour une taille donnee :
import sys
import os
import subprocess

min_time=[]
min_time_file=open("min_time_seq.txt","r")
it=0
#remplissage de la liste avec les valeurs du fichier
for line in min_time_file:
    min_time.append(float(line))
    it+=1
min_time_file.close()


acc_file_name="acc_th_fixed"+ sys.argv[1] + ".txt"
eff_file_name="eff_th_fixed"+ sys.argv[1] + ".txt"
#ouverture en ecriture des deux fichiers et ecriture de l'entete
acc_file=open(acc_file_name,"w")
acc_file.write("Taille_mat;Run_ID;Acc"+"\n")
eff_file=open(eff_file_name,"w")
eff_file.write("Taille_mat;Run_ID;Eff"+"\n")

for i in range(0,7):
    #generer un vecteur avec la nouvelle taille
    size=10**i
    nom_fichier_sortie="test_"+str(i)+".txt"
    #out_useless=subprocess.check_output("./creer_vecteur --seed 42 --size size > nom_fichier_sortie",shell=True)
    #out_useless.decode("utf-8")
    for j in range(100):
        #execution de ./tri_thread sys.argv[1] size NPO utiliser le nouveau vecteur
        out=subprocess.check_output("./tri_threads --parallelism " + sys.argv[1] + " --rusage < " +nom_fichier_sortie,shell=True)
        out.decode("utf-8")
        #print(out)
        #recuperer la valeur de retour 
        acc=min_time[i-0]/float(out)
        eff=acc/float(sys.argv[2])
        #ecriture dans fich_acc -> 10^i;j;acc
        acc_file.write(str(size)+";"+str(j)+";"+str(acc)+"\n")
        #ecriture dans fich_eff -> 10^i;j;eff
        eff_file.write(str(size)+";"+str(j)+";"+str(eff)+"\n")

acc_file.close()
eff_file.close()
print("execution terminee")
