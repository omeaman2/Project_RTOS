from scipy.io import wavfile
import numpy as np
import os

jump = 1

a = wavfile.read('../../../resources/train_short.wav')
data = np.array(a[1], dtype=float)

fdata_array = '../../../resources/data_array.txt'

print("data:\t\t", data)
f = open('../../../resources/data_array.txt', "w")
f = open(fdata_array, "w")
for i in range(0, data.size, jump):
    # if (i == data.size - 1):
        # f.write("%d" % data[i])
    # else:
    f.write("%d," % data[i])

f.close()

# Strip off last comma in data_array.txt.
with open(fdata_array, 'rb+') as f:
    f.seek(-1, os.SEEK_END)
    f.truncate()
