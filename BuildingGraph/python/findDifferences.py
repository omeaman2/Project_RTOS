import matplotlib.pyplot as plt
import numpy as np
import wave
import sys

# Trying to find the differences between the hundPercent array and the original
def findMissing(a, b): 
    diffs = []

    for i in np.nditer(a): 
        if (a[i] != b[i]): 
            diffs.append(a[i])
        else:
            diffs.append(0)

    return diffs


spf1 = wave.open("/Users/barendmosch/source/repos/Project_RTOS/comparison/inverting/wavs/doing_nothing.wav", "r")
spf2 = wave.open("/Users/barendmosch/source/repos/Project_RTOS/comparison/inverting/wavs/100percent.wav", "r")

#spf1 = wave.open("/Users/barendmosch/source/repos/Project_RTOS/comparison/inverting/wavs/doing_nothing.wav", "r")
#spf2 = wave.open("/Users/barendmosch/source/repos/Project_RTOS/comparison/setting_to_zero/wavs/100percent.wav", "r")

fs = spf1.getframerate()

original = spf1.readframes(-1)
original = np.fromstring(original, "Int16")

hundPercent = spf2.readframes(-1)
hundPercent = np.fromstring(hundPercent, "Int16")

# Declare the x axis to the time of the wav file
time = np.linspace(0, len(original) / fs, num=len(original))

# Call the find Missing method the find the differences
diffs = findMissing(original, hundPercent)

plt.figure(1)
plt.title("Comparison 100 Percent and original%")

#plt.plot(time, hundPercent, 'r')
#plt.plot(time, original, 'b')

#time = np.linspace(0, len(diffs) / fs, num=len(diffs))

plt.plot(time, diffs, 'g')
plt.show()
