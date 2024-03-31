#!/bin/bash

    # 定義所有可能的參數組合
    tau=( "2" "3")
    dataset=("CH1.txt")
    mode=("1" "2" "4")
    # 遍歷所有參數組合
    for a1 in "${dataset[@]}"; do
        for a2 in "${tau[@]}"; do
            for a3 in "${mode[@]}"; do
                # 執行程式 ./main 並傳遞參數
                echo "Running ./main with args: $a1 $a2 $a3 5"
                ./main.exe $a1 $a2 $a3 5
                echo ""
            done
        done
    done
    read -n 1