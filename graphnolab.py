import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
import pandas as pd
import sys

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 4:
    print("Please provide exactly three filenames as command-line arguments.")
    print("python3 grapy.py indegfile boolfile configfile")
    sys.exit(1)

color = ['r','g','b','black']

# Retrieve the filenames from the command-line arguments
filename1 = sys.argv[1]
filename2 = sys.argv[2]
filename3 = sys.argv[3]

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

# Step 1: Read the text file
df = pd.read_csv(filename2, delimiter=" ",index_col=0)

# Step 2: Show the table plot
fig, ax = plt.subplots(figsize=(5,8))  # Adjust the figure size as needed
ax.axis('off')  # Turn off the axis
table = pd.plotting.table(ax, df, bbox=[0, 0, 1, 1])
table.scale(1.2, 1.2)  # Adjust the table scale as needed\
plt.show()


graph.clear() # clear for the next graph
# Step 1: Parse the text and extract node and edge information
data = np.loadtxt(filename3,dtype=int)
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

# Step 5: Visualize the graph

pos = nx.nx_agraph.graphviz_layout(graph, prog="twopi", args="")
nx.draw(graph, pos, with_labels=False, node_color='lightblue', edge_color='black')
plt.show()