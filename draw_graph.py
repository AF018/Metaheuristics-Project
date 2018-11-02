# Script for showing the graph upon which the algorithm is to be run

import argparse
import numpy as np
import matplotlib
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='Get the input file')
parser.add_argument('--input', type=str, help='name of the input file', required = True)
parser.add_argument('--rcapt', type=int, help='captation radius', required = True)
parser.add_argument('--rcom', type=int, help='communication radius', required = True)
parser.add_argument('--solution', type=str, help='name of the solution file', required = True)
parser.add_argument('--output', type=str, help='name of the output file',default = "")

args = parser.parse_args()

coords = []
solution = []

## read the data file
with open(args.input, 'r') as read_file :
    for line in read_file:
        line_decomposition = line.replace("\n", "").split(" ")
        coords += [[float(line_decomposition[3]), float(line_decomposition[4])]]

## read the solution file
with open(args.solution, 'r') as read_solution :
    for line in read_solution:
        solution.append(int(line))
    #solution = [int(i) for i in read_solution.read().split(" ")[:-1]]    
            

coords = np.array(coords)
fig, ax = plt.subplots(figsize=(50,35))

## set the limits for the x and y axis
max_coords_x = max(coords[:,0])
max_coords_y = max(coords[:,1])
ax.set_xlim(left=-max_coords_x *0.05,right=max_coords_x*1.05)
ax.set_ylim(bottom=-max_coords_y*0.05,top=max_coords_y*1.05)


## display the captation graph
segs=[]
for i in range(coords.shape[0]-1):
    for j in range(i+1,coords.shape[0]):
        x=coords[i][0] - coords[j][0]
        y=coords[i][1] - coords[j][1]
        if x*x+y*y <= args.rcapt**2:
            #plt.plot((coords[i][0],coords[j][0]),(coords[i][1],coords[j][1]),color='m',linestyle='solid')
            segs.append(((coords[i][0], coords[i][1]), (coords[j][0], coords[j][1])))
ln_coll = matplotlib.collections.LineCollection(segs, colors=(0.8,0.8,0.3))
ax.add_collection(ln_coll)


## display the captation radius on vertices from solution, and the edges in the communication constituting the connexity
for vertex in solution:
    vertex_coords=coords[vertex]
    ax.add_artist(plt.Circle(vertex_coords,args.rcapt,color=(0,0.2,0.5,0.2)))
    for neighbor_vertex in solution:
        neighbor_coords = coords[neighbor_vertex]
        x=vertex_coords[0] - neighbor_coords[0]
        y=vertex_coords[1] - neighbor_coords[1]
        if x*x+y*y <= args.rcom**2:
            plt.plot([vertex_coords[0], neighbor_coords[0]], [vertex_coords[1], neighbor_coords[1]], 'r-')

plt.plot(coords[:,0],coords[:,1],"o",color=(0.2,0.2,0.2))


if args.output != "" :
    plt.savefig(args.output,quality=95)
plt.show()
