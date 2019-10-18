from scipy.io.wavfile import read
import numpy

a = read("train2.wav")
dataarray = numpy.array(a[1], dtype=float)
length = dataarray.size
print(length)
f = open("result.txt", "w+")
for i in range(0, length):
    f.write("%d," % dataarray[i])
f.close()
