# https://dsp.stackexchange.com/questions/49460/apply-low-pass-butterworth-filter-in-python

import matplotlib.pyplot as plt
import numpy as np
import sys
import soundfile as sf
import wave
from random import randint

data, samplerate = sf.read('train2.wav')
spf = wave.open('train2.wav', 'r')
fs = spf.getframerate()

spf.close()

time = np.linspace(0, len(data)/fs, num=len(data))

dataNorm = data - np.mean(data)

#acf = sm.tsa.acf(dataNorm, nlags=len(dataNorm))

periodic = []
# Filter the dum dum dum dum sound hardcoded out of the train2.wav file
for i in range(len(dataNorm)):
	if dataNorm[i] >= 0.4 or dataNorm[i] <= -0.4:
		periodic.append(dataNorm[i])
	else:
		periodic.append(0)

# Write the dum dum dum dum sound to output.wav
sf.write('output.wav', periodic, samplerate)

plt.xlabel('Lags (seconds)')
plt.ylabel('Autocorrelation')
plt.figure(1)
lag = np.arange(len(dataNorm)) / 2. / 24.
plt.title('Some Wave')
plt.plot(time, periodic)
plt.show()

