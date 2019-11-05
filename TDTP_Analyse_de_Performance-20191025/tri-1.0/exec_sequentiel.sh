#!/bin/bash
let "i = 0"
let "min = 10000000"
while [ $i -le 20]
do
    let "j = 0"
    while [ $j -le 99 ]
    do
        #il faut que le programme renvoie la valeur
        let "new_val = ./tri_sequentiel $i "
        if [ $new_val -lt $min]
        then
            let "min = new_val"
        fi
        let "j = j + 1"
    done
    echo $i + $min > tmp_seq_min.txt
    let"i = i + 1"
done