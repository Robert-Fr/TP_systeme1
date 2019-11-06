import os

#execute 100 fois l'algo sequentiel pour chaque taille voulu : 10^5 -> 10^20
min=1000000
#ouverture du fichier de sortie en ecriture
output_file_name="min_time_seq.txt"
output=open(output_file_name,"w")

for i in range(5,20):
    size=10**i
    for j in range(100):
        val=os.system("./tri_seq size")
        if val<min:
            min=val
    #ecrire dans fichier de sortie la valeur et saut de ligne
    output.write(str(val))
    
output.close()  
print("exécution terminée avec succès, valeur disponibles dans ... .txt ")
