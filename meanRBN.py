import sys
import numpy as np
import matplotlib.pyplot as plt

if (len(sys.argv) == 4):
    nSIG = int(sys.argv[1])
    infile = sys.argv[2]
    outfile= sys.argv[3]
else:
    print ("Input arguments: meanRBn.py nSIG inputfile outputfile")
    sys.exit()


Mxyz, Mxy, Sxyz, Sxy, Sx = np.loadtxt(infile, delimiter="\t", usecols=(0,1,2,3,4), unpack=True)

Mxyzm = np.mean(Mxyz)
Mxym = np.mean(Mxy)
Sxyzm = np.mean(Sxyz)
Sxym = np.mean(Sxy)
Sxm = np.mean(Sx)
Mxyze = np.std(Mxyz, ddof=1) / np.sqrt(np.size(Mxyz))
Mxye = np.std(Mxy, ddof=1) / np.sqrt(np.size(Mxy))
Sxyze = np.std(Sxyz, ddof=1) / np.sqrt(np.size(Sxyz))
Sxye = np.std(Sxy, ddof=1) / np.sqrt(np.size(Sxy))
Sxe = np.std(Sx, ddof=1) / np.sqrt(np.size(Sx))

with open(outfile, 'w') as file:
    file.write('{:.6f}\t{:.6f}\t{:.6f}\t{:.6f}\t{:.6f}\t{:.6f}\t{:.6f}\t{:.6f}\t{:.6f}\t{:.6f}\n'.format(Mxyzm, Mxyze*nSIG, Mxym, Mxye*nSIG, Sxyzm, Sxyze*nSIG, Sxym, Sxye*nSIG, Sxm, Sxe*nSIG))