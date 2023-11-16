import os
import sys
import glob
import numpy as np
import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 10:
    print("Please provide exactly three filenames as command-line arguments.")
    print("python3 Confgraph.py ALfile BFfile Cfile Mfile numS nodesize edgewidth figuresize pngpath")
    sys.exit(1)


# Retrieve the filenames from the command-line arguments
ALfile = sys.argv[1]
BFfile = sys.argv[2]
Cfile = sys.argv[3]
Mfile = sys.argv[4]
numS  = int(sys.argv[5])
size  = int(sys.argv[6])
width = float(sys.argv[7])
figs  = int(sys.argv[8])
pngpath = sys.argv[9]

# Boolean function
BF = []
l = 0
with open(BFfile, 'r') as file:
    for line in file:
        if not line.startswith('G'):
            l = l+1
            if l == numS:
                BF.append([int(num_str) for num_str in line.split()])              
BF = np.array(BF[0])

# Attractor length
AL = []
l = 0
with open(ALfile, 'r') as file:
    for line in file:
        if not line.startswith('G'):
            for element in line.split():
                l = l+1
                if l == numS:
                    AL.append(int(element))
AL = np.array(AL)

# MI
MI = []
l = 0
with open(Mfile, 'r') as file:
    for line in file:
        if not line.startswith('G'):
            for element in line.split():
                MI.append(float(element))
MI = np.array(MI)

fitness = []
for i in range(0,6000,1000):
    TotalMI = sum(MI[i:i+1000])
    fitness.append(MI[i:i+1000]/TotalMI)
fitness = np.array(fitness)
fit_flat = fitness.flatten()

connect =[]
with open(Cfile, 'r') as file:
    for line in file:
        connect.append([int(num_str) for num_str in line.split()])
connect = np.array(connect)

N = len(connect)
K = len(connect[0])
F = np.power(2,len(connect[0]))

node = np.zeros(N, dtype=np.int8)
mnode= np.zeros(N, dtype=np.int8)
confignet = np.zeros(np.power(2,N), dtype=np.int32)
functions = np.zeros((N, F), dtype=np.int8)

# function for finding the evolution of next config
def Evolution():
    for il in range(N):
        nl=0
        for jl in range(len(connect[0])):
            nl=nl+node[connect[il][jl]]*np.power(2,jl)
        mnode[il] = functions[il][nl]
    # Refreshing old configuration into new one
    for il in range(N):
        node[il] = mnode[il]
        
        
# Dec to Binary
for i in range(N):
    tmp = BF[i]
    for j in range(F):
        b=int(tmp%2)
        tmp/=2
        functions[i][j] = b
# Finding the config graph
for j in range(np.power(2,N)):
    num = 0
    k = j
    for i in range(N):
        node[i] = k % 2
        k = k/2
    Evolution()
    for i in range(N):
        num = num + node[i]*np.power(2,i)
    confignet[j] = num
    
### Drawing Config graph###
# Set up the graph
graph = nx.DiGraph()  # Create a directed graph
# Add nodes
graph.add_nodes_from(range(N))
# Add edges
edges = []
for i in range(len(confignet)):
    edges.append((i,confignet[i]))
graph.add_edges_from(edges,color='black')

# Finding attractors/cycles
attractors = list(nx.simple_cycles(graph))
edge_att = []
node_att = []
for i in range(len(attractors)):
    if len(attractors[i]) == AL[0]:
        for e in list(nx.utils.pairwise(attractors[i])):
            edge_att.append(e)
        edge_att.append((attractors[i][-1], attractors[i][0]))
        node_att= attractors[i]


# Step 5: Visualize the graph
pos = nx.nx_agraph.graphviz_layout(graph, prog="twopi", args="")
plt.figure(figsize=(figs,figs))
nx.draw(graph, pos, with_labels=False, node_size=size, node_color='lightblue', edge_color='black', width=width)
nx.draw_networkx_edges(graph, pos, edgelist=edge_att, width=width*3, edge_color="r", connectionstyle='arc3,rad=.1')
nx.draw_networkx_nodes(graph, pos, nodelist=node_att, node_size=size*10, node_color="r", label=node_att)
plt.text(0, 0, 'G=10^%d, S=%d\nAL=%d\nMI=%f\nfit=%f' %(int(numS/1000)-1, numS-int(numS/1000)*1000, AL[0], MI[numS-1], fit_flat[numS-1]),  fontsize = 12, color = 'black')
Gpngname = pngpath+'G' + str(numS) + os.path.basename(ALfile) + '.png'
plt.savefig(Gpngname, bbox_inches='tight')

states = np.zeros((len(node_att), N), dtype=np.int32)

# Dec to Binary for states
for i in range(len(node_att)):
    tmp = node_att[i]
    for j in range(N):
        b=int(tmp%2)
        tmp/=2
        states[i][j] = b


### Drawing Attractor states graph###
Nt, Nx = states.shape
fig = plt.figure(figsize=(figs,figs))
plt.imshow(states, cmap="gray")
plt.ylabel("Time")
plt.xlabel("node's state")
ax = plt.gca()
ax.set_xticks(range(Nx))
ax.set_yticks(range(Nt))
Apngname = pngpath+'A' + str(numS) + os.path.basename(ALfile) + '.png'
plt.savefig(Apngname, bbox_inches='tight')

nodesN, linksN = connect.shape
edges = []
nodes = []

for l in range(0,linksN):
    edges.append([])
    for n in range(0,nodesN):
        edges[l].append((connect[n][l],n))
for n in range(0,nodesN):
    nodes.append(n)
edges = np.array(edges)
nodes = np.array(nodes)

color = ['r','g','b','y','cyan']
connectstyle=['arc3,rad=0.00', 'arc3,rad=.1', 'arc3,rad=.2']
graph = nx.DiGraph()
graph.add_nodes_from(nodes)
pos=nx.circular_layout(graph, scale=3)

### Drawing Structure graph###
plt.figure(figsize=(figs,figs))
nx.draw_networkx_nodes(graph, pos, node_color='y')
nx.draw_networkx_labels(graph, pos)
for n in range(0,nodesN):
    for l in range(0,linksN):
        graph.add_edge(connect[n][l], n)
        nx.draw_networkx_edges(graph, pos, edgelist=[(connect[n][l], n)], edge_color=color[l], connectionstyle=connectstyle[l])


BFbinl = []
for i in BF:
    BFbin = (bin(int(i)).zfill(F+2)).replace('0b','')[::-1]
    BFbinl.append(BFbin)

df = np.column_stack((BF, BFbinl))
df = np.column_stack((connect, df))
df = pd.DataFrame(df)
table = 'I1:01010101\nI2:00110011\nI3:00001111'
columns = ('Input 1', 'Input 2', 'Input 3', 'BFDec', 'BFDec')
rows = ['N %d' % x for x in range(nodesN)]
plt.table(cellText=df.values ,rowLabels=rows, colLabels=columns, colColours=color[:K+2], loc='bottom')
plt.subplots_adjust(bottom=0.2)
plt.axis("off")
Cpngname = pngpath+'C' + str(numS) + os.path.basename(Cfile) + '.png'
plt.savefig(Cpngname, bbox_inches='tight')