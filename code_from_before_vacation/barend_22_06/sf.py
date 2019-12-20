# https://dsp.stackexchange.com/questions/49460/apply-low-pass-butterworth-filter-in-python

import numpy as np
import sys
import soundfile as sf
import wave

data, samplerate = sf.read('train2.wav')
spf = wave.open('train2.wav', 'r')
fs = spf.getframerate()

spf.close()

dataNorm = data - np.mean(data)

periodic = []
for i in range(len(dataNorm)):
	periodic.append(dataNorm[i])

sf.write('output.wav', periodic, samplerate)

