import numpy as np
import networkx as nx
import sys
from itertools import combinations

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 2:
    print("Please provide exactly one filename as command-line arguments.")
    print("python3 Mutual.py configfile")
    sys.exit(1)

# Retrieve the filenames from the command-line arguments
filename1 = sys.argv[1]

# Step 1: Parse the text and extract node and edge information
data = np.loadtxt(filename1,dtype=int)
nodes = data[:,0]
edges = []
for i in data[:,0]:
    edges.append((data[i,0],data[i,1]))


# Step 2: Set up the graph
graph = nx.DiGraph()  # Create a directed graph

# Step 3: Add nodes
graph.add_nodes_from(nodes)

# Step 4: Add edges
graph.add_edges_from(edges,color='black')

#Finding attractors
attractors = list(nx.simple_cycles(graph))

#If all attractors are fixed point. There's no need to measure MI
flag = 0
for attractor in attractors:
    flag = len(attractor) + flag
if flag == len(attractors):
    print ("0 0 0 %s" %(filename1))
    exit()


edge_att = []
for i in range(len(attractors)):
    if len(attractors[i]) == 1:
        edge_att.append((attractors[i][0],attractors[i][0]))
    else:
        for e in list(nx.utils.pairwise(attractors[i])):
            edge_att.append(e)
        edge_att.append((attractors[i][-1], attractors[i][0]))


ancestors_of_attractor = []
for i in range(len(attractors)):
    if len(attractors[i]) != 1:
        ancestors_of_attractor.append([])
        for j in attractors[i]:
            ancestors_of_attractor[i]=ancestors_of_attractor[i]+list(nx.ancestors(graph,j))
        ancestors_of_attractor[i]=list(set(ancestors_of_attractor[i]))
    else:
        ancestors_of_attractor.append(list(nx.ancestors(graph,attractors[i][0])) + attractors[i])

maxnum = len(bin(len(nodes))) - 1
zerostr = "0"
bin_attractors = []
for index, attractor in enumerate(attractors):
    bin_attractors.append([])
    for eindex, element in  enumerate(attractor):
        tmpstr = ""
        for i in range(maxnum -len(bin(element))):
            tmpstr = tmpstr + zerostr
        bin_attractors[index].append(bin(element).replace("0b",tmpstr))

nodenum = maxnum - 2

Pxlist = []
Pxylist = []
for i, attractor in enumerate(bin_attractors):
    Pxlist.append(np.zeros((nodenum,1)))
    Pxylist.append(np.zeros((int(nodenum*(nodenum-1)/2), 4)))
    for j, element in enumerate(attractor):
        for k, dig in enumerate(element):
            if dig == "1":
                Pxlist[i][k] = Pxlist[i][k]+1
    
        for l, comb in enumerate(list(combinations(element, 2))):
            if comb[0] + comb[1] == "00":
                Pxylist[i][l][0] = Pxylist[i][l][0]+1
            elif comb[0] + comb[1] == "01":
                Pxylist[i][l][1] = Pxylist[i][l][1]+1
            elif comb[0] + comb[1] == "10":
                Pxylist[i][l][2] = Pxylist[i][l][2]+1
            else:
                Pxylist[i][l][3] = Pxylist[i][l][3]+1
    Pxlist[i] = Pxlist[i]/len(attractor)
    Pxylist[i] = Pxylist[i]/len(attractor)
    
Sx = []
Sxy = []
I= []
for i, attractor in enumerate(bin_attractors):
    Sx.append(np.zeros((nodenum,1)))
    Sxy.append(np.zeros((int(nodenum*(nodenum-1)/2), 1)))
    I.append([0])
    g = 0
    for j, Px in enumerate(Pxlist[i]):
        if Px != 0 and Px != 1:
            Sx[i][j] = Sx[i][j] - (Px)*np.log2(Px) - (1-Px)*np.log2(1-Px)
    for k, Pxynode in enumerate(Pxylist[i]):
        for l, Pxy in enumerate(Pxynode):
            if Pxy != 0 and Pxy != 1:
                Sxy[i][k] = Sxy[i][k] - (Pxy)*np.log2(Pxy)
    for j, Sa in enumerate(Sx[i]):
        for l in range(j+1, len(Sx[i])):
            I[i] = I[i] + Sx[i][j] + Sx[i][l] - Sxy[i][g]
            g = g + 1
            
Itotal = 0
Sxtotal = 0
Sxytotal = 0
for idx, basin in enumerate(ancestors_of_attractor):
    Itotal = Itotal + (I[idx]*len(attractors[idx]))/(len(basin)*(int(nodenum*(nodenum-1)/2)))
    Sxytotal = Sxytotal + sum(Sxy[idx])*len(attractors[idx])/(len(basin)*int(nodenum*(nodenum-1)/2))
    Sxtotal = Sxtotal + sum(Sx[idx])*len(attractors[idx])/(len(basin)*nodenum)
print ("%7f %7f %7f %s" % (Itotal, Sxtotal, Sxytotal, filename1))