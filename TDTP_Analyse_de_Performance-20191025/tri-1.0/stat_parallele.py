#arguments : <nb_threads> <nb_proc>
#ouverture du fichier contenant les temps minimum pour une taille donnée :
import sys
import os

min_time=[]
min_time_file=open("min_time_seq.txt","r")
it=0
#remplissage de la liste avec les valeurs du fichier
for line in min_time_file:
    min_time[it]=float(line)
    it+=1
min_time_file.close()

acc_file_name="acc_"+ sys.argv[1] + ".txt"
eff_file_name="eff_"+ sys.argv[1] + ".txt"
#ouverture en écriture des deux fichiers et ecriture de l'entete
acc_file=open(acc_file_name,"w")
acc_file.write("Taille_mat;Run_ID;Acc")
eff_file=open(eff_file_name,"w")
eff_file.write("Taille_mat;Run_ID;Eff")

for i in range(5,20):
    size=10**i
    for j in range(100):
        #execution de ./tri_thread sys.argv[1] size
        res=os.systeme("./tri_thread sys.argv[1] size")
        #recupérer la valeur de retour 
        acc=min_time[i-5]/res
        eff=acc/sys.argv[2]
        #ecriture dans fich_acc -> 10^i;j;acc
        acc_file.write(str(size)+";"+str(j)+";"+str(acc))
        #ecriture dans fich_eff -> 10^i;j;eff
        eff_file.write(str(size)+";"+str(j)+";"+str(eff))

acc_file.close()
eff_file.close()
print("execution terminée")