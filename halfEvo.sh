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
dnindeg=indeg/N"$N"K"$K"
dnboolf=boolf/N"$N"K"$K"p"$p"
dnconfg=config/N"$N"K"$K"p"$p"
dirMI=MI/N"$N"K"$K"p"$p"
echo $dirname
mkdir -p $dnindeg $dnboolf $dnconfg $dirMI

# Generate the initial networks 
./Gindeg.x $N $K $st $dnindeg/st$st

Gen=0

# Generate random population
for bs in $( seq -w $sb -1 $(( sb - 5 )) ); do
  ./Gboolf.x $N $K $p $bs $dnboolf/sb"$bs"G"$Gen"
  ./Gconfg.x $N $K $dnindeg/st$st $dnboolf/sb"$bs"G"$Gen" $dnconfg/st"$st"sb"$bs"G"$Gen"
  python3 Mutual.py $dnconfg/st"$st"sb"$bs"G"$Gen"
done > $dirMI/st"$st"G"$Gen"

# Finding fitness(Fitness = MI/sMI)
awk '{print $1, FILENAME}' $dirMI/st"$st"G"$Gen" | sed 's/st/ /' | sed 's/sb/ /' | sed 's/G/ /' | awk '{print $1,$3,$4,$5}' | sort -r -k1 > $dirMI/G"$Gen"

good=`cut -d " " -f3 $dirMI/G"$Gen" | head -5`

for g in $good; do
  echo $g
  cp $dnboolf/sb"$g"G"$Gen" $dnboolf/sb"$g"G"$(( Gen + 1 ))"
  cp $dnconfg/st"$st"sb"$g"G"$Gen" $dnconfg/st"$st"sb"$g"G"$(( Gen + 1 ))"

  ./Cboolf.x $N $K $(( g - 1000 )) $dnboolf/sb"$g"G"$Gen" $dnboolf/sb"$(( g - 1000 ))"G"$(( Gen + 1 ))"
  ./Gconfg.x $N $K $dnindeg/st$st $dnboolf/sb"$(( g - 1000 ))"G"$(( Gen + 1 ))" $dnconfg/st"$st"sb"$(( g - 1000 ))"G"$(( Gen + 1 ))"
done