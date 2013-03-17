#!/usr/bin/env python

import sys

import numpy as np
import matplotlib.pyplot as pyplot

SampleRate = 96000
Samples = 400000
FreqMax = 20000

clims = None

def plot_me(signal, i, imax, MySampleRate = SampleRate, NFFT = 8192, noverlap = 1024):
  global clims
  a = pyplot.subplot(imax, 1, i + 1)
  (Pxx, freqs, bins, im) = pyplot.specgram(signal, NFFT = NFFT, Fs = MySampleRate, noverlap = noverlap )
  if clims is None:
    Pxx = 10 * np.log10(Pxx)
    clims = Pxx.min(), Pxx.max()
  pyplot.clim(*clims)
  pyplot.colorbar()

signal = np.fromfile(sys.argv[1])
signal2 = np.fromfile(sys.argv[2])

plot_me(signal, 0, 2)
plot_me(signal2, 1, 2)

pyplot.show()
