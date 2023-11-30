#!/bin/bash
# This program is for TEmain.x

# Check the number of command-line arguments
if [ $# -ne 13 ]; then
    echo "Error: Expected 13 arguments."
    echo "Usage: $0 <N> <K> <S> <G> <p> <eps> <alp> <seedSi> <seedSf> <seedBi> <seedBf> <seedIi> <seedIf>"
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
seedSi=$8
seedSf=$9
seedBi=${10}
seedBf=${11}
seedIi=${12}
seedIf=${13}

dir=../RBNdata/TE/N"$N"K"$K"S"$S"G"$G"p"$p"eps"$eps"alp"$alp";
echo $dir $seedSi $seedSf $seedBi $seedBf $seedIi $seedIf;
mkdir -p $dir
mkdir -p $dir/CFS $dir/BFS $dir/IFS $dir/MFS $dir/AFS

time for seedS in $(seq $seedSi 1000 $seedSf); do
  for seedB in $(seq $seedBi 1000 $seedBf); do 
    for seedI in $(seq $seedIi 1000 $seedIf); do 
      CF="$dir"/CFS/S$seedS;
      BF="$dir"/BFS/S"$seedS"B"$seedB"I"$seedI";
      IF="$dir"/IFS/S"$seedS"B"$seedB"I"$seedI";
      MF="$dir"/MFS/S"$seedS"B"$seedB"I"$seedI";
      AF="$dir"/AFS/S"$seedS"B"$seedB"I"$seedI";
      ./TEmain.x $N $K $S $G -$seedS -$seedB -$seedI $p $eps $alp $CF $BF $IF $MF $AF;
    
    done;
  done;
done
