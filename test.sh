#!/bin/bash

    #element
    tau=( "2") #"3"
    dataset=("CG1.txt")
    mode=("0" "1" "2" "3" "4") #"0" "1" "2" "3" 
    #run code
    for a1 in "${dataset[@]}"; do
        for a2 in "${tau[@]}"; do
            for a3 in "${mode[@]}"; do
                # run ./main.exe dataset tau mode r
                echo "Running ./main with args: $a1 $a2 $a3 5"
                ./main.exe $a1 $a2 $a3 5
                echo ""
            done
        done
    done
    #stop code and stall view 
    read -n 1