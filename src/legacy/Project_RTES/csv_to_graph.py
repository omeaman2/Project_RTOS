import csv
import matplotlib.pyplot as plt

with open('average.csv', newline='') as f:
    reader = csv.reader(f)
    
    # Contents are returned as [['value1', 'value2', ...]]
    contents = list(reader)

    # Get the inner list of contents
    content = list(contents[0])
    
    # Last item is '', remove it
    content.pop()

    # Convert the list of strings to a list of ints
    averages = [int(i) for i in content]

print(averages)

# Plot, use the index of the last as x-value
plt.scatter(range(len(averages)), averages, color="r")
plt.plot(range(len(averages)), averages, color="b")
plt.show()
