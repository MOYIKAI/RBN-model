import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import sys

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 2:
    print("Please provide exactly three filenames as command-line arguments.")
    print("python3 NetStr.py indegfile")
    sys.exit(1)

color = ['r','g','b','black']

# Retrieve the filenames from the command-line arguments
filename1 = sys.argv[1]

# Step 1: Parse the text and extract node and edge information
data = np.loadtxt(filename1,dtype=int)
nodes = data[:,-1]
nodesN, link = data.shape
link = link - 1
edge = []
for l in range(0,link):
    edge.append([])
    for n in nodes:
        edge[l].append((data[n,l],data[n,-1]))


# Step 2: Set up the graph
graph = nx.DiGraph()  # Create a directed graph

# Step 3: Add nodes
graph.add_nodes_from(nodes)

# Step 4: Add edges
for l in range(0,link):
    graph.add_edges_from(edge[l],color=color[l])
    
edges = graph.edges()
colors = [graph[u][v]['color'] for u,v in edges]


# Step 5: Visualize the graph
pos=nx.circular_layout(graph)
nx.draw(graph, pos, with_labels=True, node_color='y', edge_color=colors)
plt.show()