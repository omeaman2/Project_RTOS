import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack
import soundfile as sf
from scipy.io import wavfile
import os

def plot_file(file):

    sample_rate, signal = wavfile.read(file)
    plt.xlabel('Time')
    plt.ylabel('Vibration (g)')
    plt.plot(signal)
    return signal, sample_rate

def write_file(data, sample_rate, file):
    wavfile.write(file, sample_rate, data)

def plot_fourier(data, sample_rate):
    # This is a two channel soundtrack, I get the first track.
    #a = data.T[0]
    # This is a one channel soundtrack.
    a = data
    # This is 16-bit track, b is now normalized on [-1,1)
    b = [(ele/2**16.)*2-1 for ele in a]
    # Calculate fourier transform (complex numbers list)
    c = scipy.fftpack.fft(b)
    freqs = scipy.fftpack.fftfreq(c.size, 1./sample_rate)
    print("freqs max: %d\nfreqs min: %d\n" % (freqs.max(), freqs.min()))
    # You only need half of the fft list (real signal symmetry)
    d = len(c)/2

    # fourier = abs(c[:int(d-1)])
    fourier = c

    plt.ylim(-1000, 1000)
    plt.xlim(0, 4000)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Vibration (g)')
    plt.plot(fourier,'r') 
    return fourier, freqs

def show_info(file):
    ob = sf.SoundFile(file)
    print(file)
    print('Sample rate: {}'.format(ob.samplerate))
    print('Channels: {}'.format(ob.channels))
    print('Subtype: {}'.format(ob.subtype))

def print_freqs(fourier, freqs):
    for x in fourier:
        print(x)
    for coef,freq in zip(fourier,freqs):
        if coef:
            print('{c:>6} * exp(2 pi i t * {f})'.format(c=coef,f=freq))

def flatten_freq(fourier, freqs, x, y):
    cut_fourier_signal = fourier.copy()
    print("fourier size: ", len(fourier))
    print("freqs size:", len(freqs))
    i = 0; j = 0
    while (i < len(fourier) and j < len(freqs)):
        if (freqs[j] >= w and freqs[j] <= z):
        # if (freqs[j] <= w and freqs[j] >= z):
            # print("cutting frequency %d at index %d:%d\n" % (freqs[j], i, j))
            cut_fourier_signal[i] = 0
        i += 1; j += 1
    i = x
    return cut_fourier_signal

def flatten_around_max_min(fourier, freqs, x, y):
    cut_fourier_signal = fourier.copy()
    min_index = np.where(freqs == freqs.min())
    max_index = np.where(freqs == freqs.max())
    print("min_index: %d\n" % (min_index))
    print("min_index value: %d\n" % (freqs[min_index]))
    print("max_index: %d\n" % (max_index))
    print("max_index value: %d\n" % (freqs[max_index]))

    # Flatten between x and y of the min_index and max_index values.
    i = min_index[0][0] - x
    middle = i
    while i <= middle + y:
        cut_fourier_signal[i] = 0
        i += 1
    i = max_index[0][0] - x
    middle = i
    while i <= middle + y:
        cut_fourier_signal[i] = 0
        i += 1
    return fourier


def plot_inv_fourier(fourier):
    inverse_fourier = scipy.fftpack.ifft(fourier)
    ifft_len = len(inverse_fourier)/2
    # Get only the real values
    #fourier = abs(inverse_fourier[:int(ifft_len-1)])
    inverse_fourier = abs(inverse_fourier[:int(ifft_len-1)])

    plt.xlabel('Time (t)')
    plt.ylabel('Vibration (g)')
    plt.plot(inverse_fourier) 
    return inverse_fourier

x = 0
y = 200000
w = -1000
z = 1000

x_interval = 1000
y_interval = 1000

####################### MAIN ########################
try:
    os.remove("./ifft_train.wav")
except OSError:
    pass

filename = "train_short.wav"
show_info(filename)
signal, sample_rate = plot_file(filename)
# plt.savefig("knock.png")

plt.figure()
fourier, freqs = plot_fourier(signal, sample_rate)
# plt.savefig("knock_fft.png")

# print_freqs(fourier, freqs)
fourier = flatten_freq(fourier, freqs, x, y)
# fourier = flatten_around_max_min(fourier, freqs, x_interval, y_interval)
plt.figure()

# Plot flattened fourier
plt.xlabel('Frequency (Hz)')
plt.ylabel('Vibration (g)')
plt.plot(fourier,'r') 
plt.figure()

inverse_fourier = plot_inv_fourier(fourier)
# plt.savefig("knock_ifft.png")

write_file(inverse_fourier, sample_rate, "ifft_train.wav")
plt.show()
