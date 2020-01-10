from scipy.io import wavfile
import numpy as np

jump = 1

samplerate, data = wavfile.read('../../../resources/train_short.wav')
#data = np.array(a[1], dtype=float)

f = open('../../../resources/data_array.txt', "w")

for i in range(0, data.size, jump):
    # if (i == data.size - 1):
        # f.write("%d" % data[i])
    # else:
    f.write("%d," % data[i])

f.close()
