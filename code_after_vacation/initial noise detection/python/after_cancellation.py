from scipy.io import wavfile
import numpy as np

# samplerate, data = wavfile.read('../../../resources/train_short.wav')
samplerate = 44100

f = open('../../../resources/new_data_array.txt', "r")
data_write = f.read().split(',')
data_write = np.asarray(data_write, dtype=np.int16)
print(data_write)

wavfile.write('../../../resources/new_data_array.wav', samplerate, data_write)
f.close()
