import matplotlib.pyplot as plt
import numpy as np
import soundfile as sf
#import statsmodels.api as sm
from statsmodels import api as sm

data, samplerate = sf.read('train_short.wav')
size = data.size
acorr = []
for shift in range(10):
    correlation = np.corrcoef(data[:-shift], data[shift:])[0, 1]
    acorr.append(correlation)

print(acorr)
#sm.graphics.tsa.plot_acf(data, lags=40)

plt.xlabel('lags (seconds)')
plt.ylabel('Autocorrelation')
plt.title('Autocorrelation of train_short.wav')
#plt.show()
