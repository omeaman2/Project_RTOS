from scipy.io import wavfile
import numpy as np
import os
import git

# Get the git root
repo = git.Repo('.', search_parent_directories=True)

jump = 1
file_wav = repo.working_tree_dir + '/resources/knock2.wav'
a = wavfile.read(file_wav)
data = np.array(a[1], dtype=np.int16)

file_txt = repo.working_tree_dir + '/resources/data_array.txt'
fdata_array = file_txt

print("data:\t\t", data)
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
