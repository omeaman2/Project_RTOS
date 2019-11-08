from scipy.io.wavfile import read
import numpy

a = read("train_short.wav")
dataarray = numpy.array(a[1], dtype=float)
length = dataarray.size
print(length)
