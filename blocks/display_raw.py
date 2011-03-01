#!/usr/bin/env python

import sys
import numpy as np
from matplotlib import pyplot
import scipy.fftpack as fft

def plot(x, y):
  pyplot.plot(x)
  pyplot.plot(y)

fs = 48000
fsin = 20000

x = np.fromfile(sys.argv[1], dtype=np.float32)
y = np.fromfile(sys.argv[2], dtype=np.float32)
plot(x, y)
print y
pyplot.show()
