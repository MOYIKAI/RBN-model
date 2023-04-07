for p in $(seq -w 0.02 0.02 0.11); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
for p in $(seq -w 0.12 0.02 0.21); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
for p in $(seq -w 0.22 0.02 0.31); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
for p in $(seq -w 0.32 0.02 0.41); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
for p in $(seq -w 0.42 0.02 0.51); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
for p in $(seq -w 0.52 0.02 0.61); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
for p in $(seq -w 0.62 0.02 0.71); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
for p in $(seq -w 0.72 0.02 0.81); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
for p in $(seq -w 0.82 0.02 0.91); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
for p in $(seq -w 0.92 0.02 0.99); do
    for rs in $(seq -f %1.0f -10000 -1 -10999); do
        for rt in $(seq -f %1.0f -10000 -1 -10999); do
            ./RBN1.x 32 2 $p $rs $rt 10000 0;done;done > K2/N32_p"$p"_S1000000_t100000;done &
