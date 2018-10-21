# Script for showing the graph upon which the algorithm is to be run

import argparse

import numpy as np
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='Get the input file')
parser.add_argument('--input', type=str, help='name of the input file')

args = parser.parse_args()

coords = []
with open(args.input, 'r') as read_file:
    for line in read_file:
        line_decomposition = line.replace("\n", "").split(" ")
        coords += [[line_decomposition[3], line_decomposition[4]]]

coords = np.array(coords)
print(coords.shape)

plt.plot(coords[:, 0], coords[:, 1], "b.")
plt.xticks([])
plt.yticks([])
plt.show()