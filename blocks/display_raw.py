#!/usr/bin/env python

import sys
import numpy as np
from matplotlib import pyplot
import scipy.fftpack as fft

def plot(x, y):
  pyplot.plot(x)
  pyplot.plot(y)

i=0

if len(sys.argv) > 3:
  i = int(sys.argv[3])
  
x = np.fromfile(sys.argv[1], dtype=np.float64)
y = np.fromfile(sys.argv[2], dtype=np.float64)
plot(x[i*100000:(i+1)*100000], y[i*100000:(i+1)*100000])
print y
pyplot.show()
