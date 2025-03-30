#!/bin/bash
awk '{sum+=$1} END {printf "平均值: %.2f\n", sum/NR}' timing.txt
