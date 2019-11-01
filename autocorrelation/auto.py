import numpy as np
import matplotlib.pylab as plt
import soundfile as sf
from scipy.io import wavfile
import os
from statsmodels.graphics.tsaplots import plot_acf

def Autocorrelation(sig):
    mean = Mean(sig)
    print 'mean: ', mean
    auto = np.empty((len(sig)/2), dtype=int)
    j=0
    for x in auto:
        n = 0
        d = 0

        for i in sig:
            xim = sig[i] - mean
            n += xim * (((sig[i] + x) % len(sig)) - mean)
            d += xim * xim

        auto[x] = n / d
        j+=1
        if ((j % 50) == 0):
            print auto[x], j

    return auto

def Mean(sig):
    sum = 0
    for x in sig:
        sum += x
    return sum / len(sig)



sample_rate, signal = wavfile.read("train_short.wav")
print 'length of the signal: ', len(signal)

plot_acf(signal, lags=100)
# y = signal - np.mean(signal)
# norm = np.sum(y ** 2)
# correlated = np.correlate(y, y, mode='full')/norm

# a = Autocorrelation(signal.tolist())

# plt.plot(correlated)
plt.title('Autocorrelation of XYZ stock price data')
plt.xlabel('Lag')
plt.ylabel('Autocorrelation')
# plt.axis('tight')
plt.show()

