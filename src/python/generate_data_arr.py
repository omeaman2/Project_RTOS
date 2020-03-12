from scipy.io import wavfile
import numpy as np
import os
import git

# Get the git root
repo = git.Repo('.', search_parent_directories=True)

jump = 1
file_wav = repo.working_tree_dir + '/resources/train_short.wav'
a = wavfile.read(file_wav)
data = np.array(a[1], dtype=np.int16)
frame_rate = np.array(a[0], dtype=np.int32)
file_txt = repo.working_tree_dir + '/resources/data_array.txt'
fdata_array = file_txt

print("data:\t\t", data)
frame_rate = (float(frame_rate) / 48000) * 960;
f = open(fdata_array, "w")
f.write("%d," % frame_rate)
for i in range(1, data.size, jump):
    # if (i == data.size - 1):
        # f.write("%d" % data[i])
    # else:
    f.write("%d," % data[i])

f.close()

# Strip off last comma in data_array.txt.
with open(fdata_array, 'rb+') as f:
    f.seek(-1, os.SEEK_END)
    f.truncate()
