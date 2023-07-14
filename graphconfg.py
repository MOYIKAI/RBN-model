import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import sys

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 5:
    print("Please provide exactly three filenames as command-line arguments.")
    print("python3 grapy.py configfile nodesize edgewidth figuresize")
    sys.exit(1)


# Retrieve the filenames from the command-line arguments
filename1 = sys.argv[1]
size  = int(sys.argv[2])
width = float(sys.argv[3])
figs  = int(sys.argv[4])

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

# Step 5: Finding attractors/cycles
attractors = list(nx.simple_cycles(graph))
edge_att = []
node_att = []
for i in range(len(attractors)):
    if len(attractors[i]) == 1:
        edge_att.append((attractors[i][0],attractors[i][0]))
        node_att.append(attractors[i][0])
    else:
        for e in list(nx.utils.pairwise(attractors[i])):
            edge_att.append(e)
            node_att.append(e[0])
        edge_att.append((attractors[i][-1], attractors[i][0]))
        node_att.append(attractors[i][-1])



# Step 5: Visualize the graph

pos = nx.nx_agraph.graphviz_layout(graph, prog="twopi", args="")
plt.figure(figsize=(figs,figs))
nx.draw(graph, pos, with_labels=False, node_size=size, node_color='lightblue', edge_color='black', width=width)
nx.draw_networkx_edges(graph, pos, edgelist=edge_att, width=width*3, edge_color="red")
nx.draw_networkx_nodes(graph, pos, nodelist=node_att, node_size=size*4, node_color="red")
plt.show()
