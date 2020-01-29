import matplotlib.pyplot as plt
import numpy as np
import wave
import sys


spf1 = wave.open("/Users/barendmosch/source/repos/Project_RTOS/comparison/inverting/wavs/doing_nothing.wav", "r")
spf2 = wave.open("/Users/barendmosch/source/repos/Project_RTOS/comparison/setting_to_zero/wavs/100percent.wav", "r")

fs = spf1.getframerate()

original = spf1.readframes(-1)
original = np.fromstring(original, "Int16")

hundPercent = spf2.readframes(-1)
hundPercent = np.fromstring(hundPercent, "Int16")

time = np.linspace(0, len(original) / fs, num=len(original))


plt.figure(1)
plt.title("Comparison 100 Percent and original")

ax = plt.subplot(111)

ax.plot(time, original, 'r', label='Original')
ax.plot(time, hundPercent, 'b', label='100 % cancalation')

ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.05), shadow=True, ncol=2)

plt.show()
