import matplotlib.pyplot as plot
from scipy.io import wavfile
import numpy as np

# Time series data
# data = np.array([24.40,10.25,20.05,22.00,16.90,7.80,15.00,22.80,34.90,13.30])
sample_rate, data = wavfile.read("knock.wav")


# Plot autocorrelation
plot.acorr(data[:10], maxlags=None)

# Add labels to autocorrelation plot
plot.title('Autocorrelation of XYZ stock price data')
plot.xlabel('Lag')
plot.ylabel('Autocorrelation')

# Display the autocorrelation plot
plot.show()
