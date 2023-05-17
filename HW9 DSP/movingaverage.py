import csv
import matplotlib.pyplot as plt
import numpy as np

t = [] # column 0
data1 = [] # column 1
data2 = [] # column 2

with open('sigD.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column
        #data2.append(float(row[2])) # third column

# for i in t:
#     # print the data to verify it was read
#     print(str(t[i]) + ", " + str(data1[i]) + ", " + str(data2[i]))

sample_rate = len(data1)/(t[-1]-t[1])
print(sample_rate)

Fs = sample_rate
Ts = 1.0/Fs
ts = np.arange(0,t[-1],Ts)
y = data1
n = len(y)
k = np.arange(n)
T = n/Fs
frq = k/T
frq = frq[range(int(n/2))]
Y = np.fft.fft(y)/n # fft computing and normalization
Y = Y[range(int(n/2))]

moving_avg = 100
t1 = []
data11 = []
for x in range(0,len(t)-moving_avg-1):
    t1.append(sum(t[int(x):int(x+moving_avg-1)])/int(moving_avg))
    data11.append(sum(data1[int(x):int(x+moving_avg)])/int(moving_avg))

sample_rate1 = len(data11)/(t1[-1]-t1[1])
Fs1 = sample_rate1
Ts1 = 1.0/Fs1
ts1 = np.arange(0,t1[-1],Ts1)
y1 = data11
n1 = len(y1)
k1 = np.arange(n1)
T1 = n1/Fs1
frq1 = k1/T1
frq1 = frq1[range(int(n1/2))]
Y1 = np.fft.fft(y1)/n1 # fft computing and normalization
Y1 = Y1[range(int(n1/2))]


fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.set_title('sigD')
ax1.plot(t,y,'b')
ax1.plot(t1,y1,'r')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax2.loglog(frq,abs(Y),'b') # plotting the fft
ax2.loglog(frq1,abs(Y1),'r')
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
plt.show()

