import os
import subprocess
#execute 100 fois l'algo sequentiel pour chaque taille voulu : 10^5 -> 10^20
#ouverture du fichier de sortie en ecriture
output_file_name="min_time_seq.txt"
output=open(output_file_name,"w")

for i in range(0,7):
    min=1000000.0
    size=10**i
    nom_fichier_sortie="test_"+str(i)+".txt"
    out_useless=subprocess.check_output("./creer_vecteur --seed 42 --size " +str(size)+ " > "+ nom_fichier_sortie,shell=True)
    out_useless.decode("utf-8")
    for j in range(100):
        val=subprocess.check_output("./tri_sequentiel --rusage < "+ nom_fichier_sortie,shell=True)
        val.decode("utf-8")
        #print(float(val))
        if float(val)<min:
            min=float(val)
    #ecrire dans fichier de sortie la valeur et saut de ligne
    output.write(str(min)+"\n")
    
output.close()  
print("execution terminee avec succes, valeur disponibles dans ... .txt ")
