# Script for creating grid dat files

import argparse

import numpy as np

parser = argparse.ArgumentParser(description='Get the input file')
parser.add_argument('--width', type=int)
parser.add_argument('--height', type=int)
parser.add_argument('--output', type=str, help='name of the output file')

args = parser.parse_args()

with open(args.output, 'w') as output_file:
    for width_idx in range(args.width):
        for height_idx in range(args.height):
            output_file.write(" " + str(height_idx + (args.height*width_idx))
                            + "  " + str(width_idx) + " " + str(height_idx) + "\n")