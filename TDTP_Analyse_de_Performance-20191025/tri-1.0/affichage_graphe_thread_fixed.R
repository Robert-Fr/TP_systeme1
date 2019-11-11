# script R permettant de comparer les temps moyens de calcul des produits de matrices carrées entre algo classique et algo transposée
# pour des tailles variant de 2000 à 5000

library(ggplot2)
library(plyr)
library(reshape2)

# extraction des données
data_para_acc <- data.frame(read.csv2(file="fichier_donnee/acc_th_fixed2.txt",sep=';', dec='.'))
data_para_eff <- data.frame(read.csv2(file="fichier_donnee/eff_th_fixed2.txt",sep=';', dec='.'))

# analyse statistique

stat_acc<-ddply(data_para_acc,c("Taille_mat"),summarise,N=length(Acc),mean=mean(Acc),sd=sd(Acc),se=1.96*(sd/sqrt(N)))
stat_eff<-ddply(data_para_eff,c("Taille_mat"),summarise,N=length(Eff),mean=mean(Eff),sd=sd(Eff),se=1.96*(sd/sqrt(N)))


# tracé des résultats obtenus

p <- ggplot()

p <- p + geom_point(data=stat_acc, aes(x=Taille_mat, y=mean, colour = "acceleration")) + geom_line(data=stat_acc, aes(x=Taille_mat, y=mean, colour = "acceleration")) 
p <- p + geom_errorbar(data=stat_acc,aes(x=Taille_mat,ymin=mean-se,ymax=mean+se),width=.2,position=position_dodge())

p <- p + geom_point(data=stat_eff, aes(x=Taille_mat, y=mean, colour = "efficacite")) + geom_line(data=stat_eff, aes(x=Taille_mat, y=mean, colour = "efficacite"))
p <- p + geom_errorbar(data=stat_eff,aes(x=Taille_mat,ymin=mean-se,ymax=mean+se),width=.2,position=position_dodge())

p <- p + xlab("Taille des matrices")
p <- p + ylab("Unite arbitraire")

cols <- c("acceleration"="red","efficacite"="blue")
p <- p + scale_colour_manual("", 
                             values = cols)

p <-  p + theme(panel.background = element_rect(fill = "lightgrey"),
                panel.grid.minor = element_line(colour = "black", linetype="dashed", size = 0.1),
                panel.grid.major = element_line(colour = "black", size = 0.1),
                legend.position="bottom")

show(p)
