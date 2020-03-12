import sys, wave
import soundfile as sf

data, samplerate = sf.read('../../resources/train_short.wav')

data_write = []
for i in range(len(data)):
	data_write.append(data[i])

sf.write('../../resources/new_data_array.wav', data_write, samplerate)
