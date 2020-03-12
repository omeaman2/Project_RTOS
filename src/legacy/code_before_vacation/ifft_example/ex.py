import numpy as np
import matplotlib.pyplot as plt
# Time period
t = np.arange(0, 10, 0.01);
# Create a sine wave with multiple frequencies(1 Hz, 2 Hz and 4 Hz)
a = np.sin(2*np.pi*t) + np.sin(2*2*np.pi*t) + np.sin(4*2*np.pi*t);
# Do a Fourier transform on the signal
tx  = np.fft.fft(a);
# Do an inverse Fourier transform on the signal
itx = np.fft.ifft(tx);
# Plot the original sine wave using inverse Fourier transform
plt.plot(t, a);
plt.figure()
plt.plot(t, itx, 'r');
plt.title("Sine wave plotted using inverse Fourier transform");
plt.xlabel('Time')
plt.ylabel('Amplitude')
plt.grid(True)
plt.show();
