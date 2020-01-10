from scipy.io import wavfile
import numpy as np

jump = 1

a = wavfile.read('../../../resources/train_short.wav')
data = np.array(a[1], dtype=float)
print("data:\t\t", data)
f = open('../../../resources/data_array.txt', "w")
for i in range(0, data.size, jump):
    # if (i == data.size - 1):
        # f.write("%d" % data[i])
    # else:
    f.write("%d," % data[i])

f.close()
