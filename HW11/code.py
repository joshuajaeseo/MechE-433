import board
import time
import random
import digitalio
import analogio
import ulab
from ulab import numpy as np # to get access to ulab numpy functions
# Declare an array with some made up data like
# a = np.array([1,3,5,2,4,8,10])
# # Test some stats functions, like
# print(np.min(a))
# print(np.max(a))
# print(np.median(a))
# Want to know all the functions available in numpy? In REPL type np. and press Tab.



t = np.arange(0.0, 1.28, .01) # 10s
# a constant plus 100Hz and 1000Hz
s =  4*np.sin(2*np.pi * t)+np.sin(2*np.pi*0.1*t)+np.sin(2*np.pi*0.001*t)

Fs = 128 # sample rate
Ts = 1.0/Fs; # sampling interval
ts = np.arange(0,t[-1],Ts) # time vector
y = s # the data to make the fft from
n = len(y) # length of the signal
k = np.arange(n)
T = n/Fs
frq = k/T # two sides frequency range
frq = frq[range(int(n/2))] # one side frequency range
Y = ulab.utils.spectrogram(y)
#Y = Y[range(int(n/2))]

def range_f(start, stop, step):
    x = start
    while x <= stop:
        yield x
        x += step

for x in range_f(0,len(Y)-1,1):
    print("("+str(Y[x])+",)")
    time.sleep(0.05)

