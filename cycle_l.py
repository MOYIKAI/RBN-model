import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import sys

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 2:
    print("Please provide exactly three filenames as command-line arguments.")
    print("python3 cycle_l.py configfile")
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

# Step 5: Finding attractors/cycles
attractors = list(nx.simple_cycles(graph))
for attractor in attractors:
    print("%d" %len(attractor), attractor)
