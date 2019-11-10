# programme regroupant l'execution de plusieurs fonction :
#nous allons calculer d'une part, pour une taille donnee dasn un ensemble de taile possible (10^1 -> 10^7), 
#l'evoluton de l'acc et l'eff du tri avec thread vs tri sequentiel, pour un ensemble de nombre de thread possible ( 2 4 8 16 32 )
#d'une autre part pour un nombre de thread donnee ( 2 4 8 16 32 ) l'evolution de l'acc et l'eff en fonction de la taille du vecteur
import sys
import os
import subprocess

#definition des fonctions qui seront utilisees :
# important : lancement du script : python Etude_statistique_tri_thread_vs_seq.py <nom_fichier_min> <nb_proc_machine> <>

def gen_min_time_seq (output_file_param="min_time_seq.txt") :
    output_file_name=output_file_param
    output=open(output_file_name,"w")

    #for i in range(0,9):
    for i in ["100000","500000","1000000","1500000","2000000","2500000","3000000","3500000","4000000","4500000","5000000"]:
        min=1000000000000000.0
        #size=10**i
        size=i
        #nom_fichier_sortie="test_"+str(i)+".txt"
        nom_fichier_sortie="test_"+i+".txt"
        #out_useless=subprocess.check_output("./creer_vecteur --seed 42 --size " +str(size)+ " > "+ nom_fichier_sortie,shell=True)
        out_useless=subprocess.check_output("./creer_vecteur --seed 42 --size " +size+ " > "+ nom_fichier_sortie,shell=True)
        out_useless.decode("utf-8")
        for j in range(100):
            #print("execution "+str(j)+" de gen_min_time_seq pour size = "+ str(size))
            print("execution "+str(j)+" de gen_min_time_seq pour size = "+ size)
            val=subprocess.check_output("./tri_sequentiel --rusage < "+ nom_fichier_sortie,shell=True)
            val.decode("utf-8")
            #print(float(val))
            if float(val)<min:
                min=float(val)
        #ecrire dans fichier de sortie la valeur et saut de ligne
        output.write(str(min)+"\n")
        
    output.close()  
    print("generation des minimum terminee avec succes, valeur disponibles dans "+ output_file_name +".txt ")
    
def calcul_stats_tri_size_fixed(size_param="5",nb_proc="8",min_time_file_param="min_time_seq.txt") :
    min_time=[]
    min_time_file=open(min_time_file_param,"r")
    it=0
    #remplissage de la liste avec les valeurs du fichier
    for line in min_time_file:
        min_time.append(float(line))
        it+=1
    min_time_file.close()

    acc_file_name="acc_size_fixed"+ size_param + ".txt"
    eff_file_name="eff_size_fixed"+ size_param + ".txt"
    #ouverture en ecriture des deux fichiers et ecriture de l'entete
    acc_file=open(acc_file_name,"w")
    acc_file.write("NB_Thread;Run_ID;Acc"+"\n")
    eff_file=open(eff_file_name,"w")
    eff_file.write("NB_Thread;Run_ID;Eff"+"\n")

    input_file_name="test_"+size_param+".txt"

    for i in ["2","4","8","16"]:
        #generer un vecteur avec la nouvelle taille
        for j in range(100):
            print("execution "+str(j)+" de calcul_stats_tri_size_fixed pour nb_th = "+ str(i))
            #execution de ./tri_thread sys.argv[1] size NPO utiliser le nouveau vecteur 
            out=subprocess.check_output("./tri_threads --parallelism " + str(i) + " --rusage < " +input_file_name,shell=True)
            out.decode("utf-8")
            #recuperer la valeur de retour 
            acc=min_time[(int(size_param)/500000)-1]/float(out)
            eff=acc/float(nb_proc)
            #ecriture dans fich_acc -> 10^i;j;acc
            acc_file.write(str(i)+";"+str(j)+";"+str(acc)+"\n")
            #ecriture dans fich_eff -> 10^i;j;eff
            eff_file.write(str(i)+";"+str(j)+";"+str(eff)+"\n")

    acc_file.close()
    eff_file.close()
    print(" - execution taille fixe pour taille = " + size_param + " terminee")

def calcul_stats_tri_th_fixed(nb_threads="4",nb_proc="8",min_time_file_param="min_time_seq.txt") :
    min_time=[]
    min_time_file=open(min_time_file_param,"r")
    it=0
    #remplissage de la liste avec les valeurs du fichier
    for line in min_time_file:
        min_time.append(float(line))
        it+=1
    min_time_file.close()


    acc_file_name="acc_th_fixed"+ nb_threads + ".txt"
    eff_file_name="eff_th_fixed"+ nb_threads + ".txt"
    #ouverture en ecriture des deux fichiers et ecriture de l'entete
    acc_file=open(acc_file_name,"w")
    acc_file.write("Taille_mat;Run_ID;Acc"+"\n")
    eff_file=open(eff_file_name,"w")
    eff_file.write("Taille_mat;Run_ID;Eff"+"\n")

    #for i in range(5,9):
    for i in ["100000","500000","1000000","1500000","2000000","2500000","3000000","3500000","4000000","4500000","5000000"]:
        #size=10**i
        size=i
        #input_file_name="test_"+str(i)+".txt"
        input_file_name="test_"+i+".txt"
        for j in range(100):
            #print("execution "+str(j)+" de calcul_stats_tri_th_fixed pour size = "+ str(size))
            print("execution "+str(j)+" de calcul_stats_tri_th_fixed pour size = "+ size)
            #execution de ./tri_thread sys.argv[1] size NPO utiliser le nouveau vecteur
            out=subprocess.check_output("./tri_threads --parallelism " +  nb_threads + " --rusage < " +input_file_name,shell=True)
            out.decode("utf-8")
            #print(out)
            #recuperer la valeur de retour 
            acc=min_time[(int(size_param)/500000)-1]/float(out)
            eff=acc/float(nb_proc)
            #ecriture dans fich_acc -> 10^i;j;acc
            #acc_file.write(str(size)+";"+str(j)+";"+str(acc)+"\n")
            acc_file.write(size+";"+str(j)+";"+str(acc)+"\n")
            #ecriture dans fich_eff -> 10^i;j;eff
            #eff_file.write(str(size)+";"+str(j)+";"+str(eff)+"\n")
            eff_file.write(size+";"+str(j)+";"+str(eff)+"\n")

    acc_file.close()
    eff_file.close()
    print(" - execution nb threads fixe = " + nb_threads + " terminee")

#appel des fonctions :
min_time_seq_file_name=sys.argv[1]
nb_proc_sys=sys.argv[2]
#on genere d'abord un fichier contenant les valeurs minimale de temps sur 100 execution que le tri sequentiel va mettre pour terminer pour chaque taille de notre ensemble
#gen_min_time_seq()
#a ce stade la on a aussi les fichiers contenants les vecteurs sur lesquels nous allons effectuer nos tri -> fichiers "test_x.txt"

#on appelle ensuite les fonction generant les fichiers desires, qui seront ensuite lu par un script R pour afficher les donnees sous forme de graphe :
for size in ["100000","500000","1000000","1500000","2000000","2500000","3000000","3500000","4000000","4500000","5000000"]:
#for size in ["5","6","7"]:
    #for size in ["5","6"]:
    calcul_stats_tri_size_fixed(size_param=size,nb_proc=nb_proc_sys)
   
for nb_thread in ["2","4","8","16","32"]:
    #for nb_thread in ["2"]:
    calcul_stats_tri_th_fixed(nb_threads=nb_thread,nb_proc=nb_proc_sys)
