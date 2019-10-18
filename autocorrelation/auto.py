import numpy as np
import matplotlib.pylab as plt
import soundfile as sf
from scipy.io import wavfile
import os

#def flatten(sig, size):

sample_rate, signal = wavfile.read("train_short.wav")
print(len(signal))

# a = flatten(signal.tolist(), len(signal))

#plt.plot(a)
plt.xlabel('Angle [rad]')
plt.ylabel('sin(x)')
plt.axis('tight')
#plt.show()
