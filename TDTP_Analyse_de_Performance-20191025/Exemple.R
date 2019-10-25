# script R permettant de comparer les temps moyens de calcul des produits de matrices carrées entre algo classique et algo transposée
# pour des tailles variant de 2000 à 5000

library(ggplot2)
library(plyr)
library(reshape2)

# extraction des données
data_para_naif <- data.frame(read.csv2(file="mesures_500_5000_1_para.csv",sep=';', dec='.'))
data_para_transposee <- data.frame(read.csv2("mesures_500_5000_1_para_transposee.csv",sep=';', dec='.'))

# analyse statistique

stat_naif<-ddply(data_para_naif,c("Taille_mat"),summarise,N=length(Time),mean=mean(Time),sd=sd(Time),se=1.96*(sd/sqrt(N)))
stat_transposee<-ddply(data_para_transposee,c("Taille_mat"),summarise,N=length(Time),mean=mean(Time),sd=sd(Time),se=1.96*(sd/sqrt(N)))


# tracé des résultats obtenus

p <- ggplot()

p <- p + geom_point(data=stat_naif, aes(x=Taille_mat, y=mean, colour = "algorithme naïf")) + geom_line(data=stat_naif, aes(x=Taille_mat, y=mean, colour = "algorithme naïf")) 
p <- p + geom_errorbar(data=stat_naif,aes(x=Taille_mat,ymin=mean-se,ymax=mean+se),width=.2,position=position_dodge())

p <- p + geom_point(data=stat_transposee, aes(x=Taille_mat, y=mean, colour = "algorithme transposée")) + geom_line(data=stat_transposee, aes(x=Taille_mat, y=mean, colour = "algorithme transposée"))
p <- p + geom_errorbar(data=stat_transposee,aes(x=Taille_mat,ymin=mean-se,ymax=mean+se),width=.2,position=position_dodge())

p <- p + xlab("Taille des matrices")
p <- p + ylab("Temps de calcul (s)")

cols <- c("algorithme naïf"="red","algorithme transposée"="blue")
p <- p + scale_colour_manual("", 
                             values = cols)

p <-  p + theme(panel.background = element_rect(fill = "lightgrey"),
                panel.grid.minor = element_line(colour = "black", linetype="dashed", size = 0.1),
                panel.grid.major = element_line(colour = "black", size = 0.1),
                legend.position="bottom")

show(p)
