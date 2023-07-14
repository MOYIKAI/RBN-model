#!/bin/bash

# Check the number of command-line arguments
if [ $# -ne 5 ]; then
    echo "Error: Expected 5 arguments."
    echo "Usage: $0 <N> <K> <p> <st> <sb>"
    exit 1
fi

# Assign command-line arguments to variables
N=$1
K=$2
p=$3
st=$4
sb=$5

# Print the values of the variables
echo N$N K$K p$p sb$sb st$st

# Making default directory 
dirname=N"$N"K"$K"p"$p"
echo $dirname
mkdir -p indeg/$dirname boolf/$dirname config/$dirname  MI/$dirname

# Generate the initial networks and their boolean functions config
for i in $(seq -w $isb -1 $fsb); do
    ./gRBN.x $N $K $p $st $sb indeg/$dirname/st"$st" boolf/$dirname/sb"$sb" config/$dirname/st"$st"sb"$sb"
python3 Mutual.py config/$dirname/st"$st"sb"$sb" > MI/$dirname/st"$st"sb"$sb"