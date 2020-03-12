import wave, struct, math, random, sys, numpy
import soundfile as sf

obj = wave.open('../../resources/train_short.wav','r')
print(type(obj))

channels = obj.getnchannels()
sample_width = obj.getsampwidth()
sample_rate = obj.getframerate()
nframes = obj.getnframes()
params = obj.getparams()

data, samplerate = sf.read('../../resources/train_short.wav')

#print(type(obj))
duration = 1.0 # seconds

frequency = 440.0 # hertz

obj2 = wave.open('../../resources/new_data_array.wav','w')
obj2.setnchannels(channels)
obj2.setsampwidth(sample_width)
obj2.setframerate(sample_rate)

# for i in range(nframes):
#    value = random.randint(-32767, 32767)
#    data = struct.pack('<h', value)
#    obj.writeframesraw(data)
data_write = []
for i in range(len(data)):
    data_write.append(data[i])


print("Number of channels:",channels)
print ("Sample width:", sample_width)
print ("Sample rate:", sample_rate)
print ("Number of frames:", nframes)
print ("parameters:", params)


sf.write('../../../resources/new_data_array.wav', 'w', data_write, samplerate)

obj.close()
obj2.close()

