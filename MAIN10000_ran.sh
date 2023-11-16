#!/bin/bash

# Check the number of command-line arguments
if [ $# -ne 8 ]; then
    echo "Error: Expected 8 arguments."
    echo "Usage: $0 <N> <K> <S> <G> <p> <eps> <alp> <number of simulations>"
    exit 1
fi
# Assign command-line arguments to variables
N=$1
K=$2
S=$3
G=$4
p=$5
eps=$6
alp=$7
num=$8

dir=../RBNdata/N"$N"K"$K"S"$S"G"$G"p"$p"eps"$eps"alp"$alp";
mkdir -p $dir
mkdir -p $dir/CFS $dir/BFS $dir/IFS $dir/MFS $dir/AFS

time for seedS in $(seq 1 1 $num); do
      seedS=`jot -r 1 10000000 99999999`
      seedB=`jot -r 1 10000000 99999999`
      seedI=`jot -r 1 10000000 99999999`
      CF="$dir"/CFS/S$seedS;
      BF="$dir"/BFS/S"$seedS"B"$seedB"I"$seedI";
      IF="$dir"/IFS/S"$seedS"B"$seedB"I"$seedI";
      MF="$dir"/MFS/S"$seedS"B"$seedB"I"$seedI";
      AF="$dir"/AFS/S"$seedS"B"$seedB"I"$seedI";
      ./main10000.x $N $K $S $G -$seedS -$seedB -$seedI $p $eps $alp $CF $BF $IF $MF $AF;
done
