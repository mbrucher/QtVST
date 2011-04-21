#!/usr/bin/env python

import sys
import numpy as np
from matplotlib import pyplot
import scipy.fftpack as fft

def plot(x, y):
  pyplot.plot(x)
  pyplot.plot(y)

x = np.fromfile(sys.argv[1], dtype=np.float64)
y = np.fromfile(sys.argv[2], dtype=np.float64)
plot(x[10*100000:11*100000], y[10*100000:11*100000])
print y
pyplot.show()
