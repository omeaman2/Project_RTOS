# Example usage: 
# python3 csv_to_graph.py -csvfile '../csv/train_short.csv'

import csv
import matplotlib.pyplot as plt
import argparse

# Handle command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("-csvfile", help="csv file to convert to graph", 
                    type=str, required=True)
args = parser.parse_args()

with open(args.csvfile, newline='') as f:
    reader = csv.reader(f)
    
    # Contents are returned as [['value1', 'value2', ...]]
    contents = list(reader)

    # Get the inner list of contents
    content = list(contents[0])
    
    # Last item is '', remove it
    content.pop()

    # Convert the list of strings to a list of ints
    averages = [int(i) for i in content]

# Plot, use the index of the last as x-value
plt.scatter(range(len(averages)), averages, color="r")
plt.plot(range(len(averages)), averages, color="b")
plt.show()
