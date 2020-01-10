from scipy.io import wavfile
import numpy as np

samplerate = 44100

f = open('../../../resources/new_data_array.txt', 'r')
new_data = f.read().split(',')
new_data = np.asarray(new_data, dtype=np.float64)
print('Will write the following:', new_data)
wavfile.write('../../fft/using_the_kissfft_library/new_data_array.wav',
        samplerate, new_data)
f.close()
