from scipy.io import wavfile
import numpy as np

a = wavfile.read('../../../resources/train_short.wav')
data = np.array(a[1], dtype=float)
print("data:\t\t", data)
f = open('../../../resources/data_array.txt', "w")
for i in range(0, data.size, 8):
    f.write("%d," % data[i])
f.close()

samplerate = 44100
f2 = open('../../../resources/new_data_array.txt', 'r')
new_data = f2.read().split(',')
new_data = np.asarray(new_data, dtype=np.float64)
print('new_data:\t', new_data)
wavfile.write('../../../resources/new_data_array.wav',
        samplerate, new_data)
f2.close()
