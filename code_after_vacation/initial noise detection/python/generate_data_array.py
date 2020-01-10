from scipy.io import wavfile
import numpy as np

jump = 8

samplerate, data = wavfile.read('../../../resources/train_short.wav')
#data = np.array(a[1], dtype=float)
print("data:\t\t", data)

data_write = []
for i in range(len(data)):
    data_write.append(data[i])

wavfile.write('../../../resources/new_data_array.wav', samplerate, data_write)

'''
f = open('../../../resources/data_array.txt', "w")
for i in range(0, data.size, jump):
    # if (i == data.size - 1):
        # f.write("%d" % data[i])
    # else:
    f.write("%d," % data[i])

f.close()
'''
# samplerate = 44100 // jump
# # samplerate = 44100

# f2 = open('../../../resources/data_array.txt', 'r')
# new_data = f2.read().split(',')
# new_data = np.asarray(new_data, dtype=np.float64)
# print('new_data:\t', new_data)
# wavfile.write('../../../resources/new_data_array.wav',
#         samplerate, new_data)
# f2.close()
