import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys

# Check if the correct number of command-line arguments is provided
if len(sys.argv) != 2:
    print("Please provide exactly three filenames as command-line arguments.")
    print("python3 Boolf.py boolfile")
    sys.exit(1)

# Retrieve the filenames from the command-line arguments
filename1 = sys.argv[1]

# Step 1: Read the text file
df = pd.read_csv(filename1, delimiter=" ",index_col=0)

# Step 2: Show the table plot
fig, ax = plt.subplots(figsize=(5,8))  # Adjust the figure size as needed
ax.axis('off')  # Turn off the axis
table = pd.plotting.table(ax, df, bbox=[0, 0, 1, 1])
table.scale(1.2, 1.2)  # Adjust the table scale as needed\
plt.show()