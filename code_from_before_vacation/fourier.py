import numpy as np
import matplotlib.pyplot as plt
import scipy.fftpack
import soundfile as sf
from scipy.io import wavfile

def plot_file(file, figure_number):

    sample_rate, signal = wavfile.read(file)
    plt.xlabel('Time')
    plt.ylabel('Vibration (g)')
    #plt.xticks(np.arange(0, 3, 1.0))
    #plt.ylim(0, 500)
    #plt.xlim(0,30000)
    plt.plot(signal)
    return signal, sample_rate

def plot_fourier(data, sample_rate, figure_number):
    # This is a two channel soundtrack, I get the first track.
    #a = data.T[0]
    # This is a one channel soundtrack.
    a = data
    #print("len of data is: %d\n" % len(data))
    # This is 16-bit track, b is now normalized on [-1,1)
    b = [(ele/2**16.)*2-1 for ele in a]
    # Calculate fourier transform (complex numbers list)
    c = scipy.fftpack.fft(b)
    # You only need half of the fft list (real signal symmetry)
    d = len(c)/2

    fourier = abs(c[:int(d-1)])

    # We need to convert the xlabel to the frequency.
    # Currently not using these three lines.
    k = np.arange(len(data))
    T = len(data)/sample_rate
    frqLabel = k/T  

    #plt.ylim(0, 2000)
    #plt.xlim(0, 30000)
    plt.ylim(0, 500)
    plt.xlim(0, 2000)
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Vibration (g)')
    plt.plot(fourier,'r') 
    return fourier

def show_info(file):
    ob = sf.SoundFile(file)
    print(file)
    print('Sample rate: {}'.format(ob.samplerate))
    print('Channels: {}'.format(ob.channels))
    print('Subtype: {}'.format(ob.subtype))

def print_fourier_freqs(original_signal, fourier_signal, sample_rate): 
    freqs = np.fft.fftfreq(len(original_signal))

    #Frequency (Hz) = abs(fft_freq * frame_rate)
    for coef,freq in zip(fourier_signal,freqs):
        if coef:
            print(abs(freq * sample_rate))
    

filename = "knock.wav"
show_info(filename)
signal, sample_rate = plot_file(filename, 1)
signal2 = signal
plt.savefig("knock_again.png")
plt.figure()
fourier = plot_fourier(signal, sample_rate, 2)
plt.savefig("knock_again_fft.png")
plt.show()

print_fourier_freqs(signal, fourier, sample_rate)
