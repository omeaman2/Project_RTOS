from scipy.io import wavfile
#import soundfile as sf
import matplotlib.pyplot as plt

################################################################################
#                            READING THE SIGNAL                                #
################################################################################
# read the .wav file. 
sr, sig = wavfile.read('train.wav')
#sig, sr = sf.read('train.wav')


# Plot the signal.
'''
x is time (apparently)
y is loudness in decibel
'''

################################################################################
#                            DO ALGORITHM                                      #
################################################################################

inv = ~sig
cancelled = sig + inv

################################################################################
#                            PLOT                                              #
################################################################################


#plt.xlim(0, 1e6)
#plt.title('Signal Wave...')

f1 = plt.figure(1)
f2 = plt.figure(2)
f3 = plt.figure(3)
# 111 means 1x1 grid, first subplot
ax1 = f1.add_subplot(111)
ax2 = f2.add_subplot(111)
ax3 = f3.add_subplot(111)
# Actually plot
ax1.plot(sig)
ax2.plot(inv)
ax3.plot(cancelled)

plt.show()
