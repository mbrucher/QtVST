#!/usr/bin/env python

import numpy as np
import scipy.signal as signal
from matplotlib import pyplot

SampleRate = 96000
Samples = 2000000
FreqMax = 20000

def plot_me(signal, i, imax, MySampleRate = SampleRate, NFFT = 8192, noverlap = 1024):
  a = pyplot.subplot(imax, 2, 2 * i + 1)
  pyplot.title("Left %i" % MySampleRate)
  pyplot.specgram(signal[0], NFFT = NFFT, Fs = MySampleRate, noverlap = noverlap )
  a = pyplot.subplot(imax, 2, 2 * (i + 1))
  pyplot.title("Right %i" % MySampleRate)
  pyplot.specgram(signal[1], NFFT = NFFT, Fs = MySampleRate, noverlap = noverlap )

t = np.arange(Samples, dtype=np.float64) / SampleRate
input1 = np.sin(np.pi * (SampleRate * FreqMax / Samples * (t + .1)) * t)
input2 = input1[::-1].flatten()

signal = np.array((input1, input2))

plot_me(signal, 0, 2)

def oversample2_6point_5_order(signal):
  signal_ex = np.hstack((np.zeros((signal.shape[0], 2)), signal, np.zeros((signal.shape[0], 3))))[:,:,None]

  even1 = signal_ex[:,2:-3] + signal_ex[:,3:-2]
  even2 = signal_ex[:,1:-4] + signal_ex[:,4:-1]
  even3 = signal_ex[:,0:-5] + signal_ex[:,5:]
  odd1 = signal_ex[:,3:-2] - signal_ex[:,2:-3]
  odd2 = signal_ex[:,4:-1] - signal_ex[:,1:-4]
  odd3 = signal_ex[:,5:] - signal_ex[:,0:-5]
  
  c0 = even1 * 0.40513396007145713 + even2 * 0.09251794438424393 + even3 * 0.00234806603570670
  c1 = odd1 * 0.28342806338906690 + odd2 * 0.21703277024054901 + odd3 * 0.01309294748731515
  c2 = -even1 * 0.191337682540351941 + even2 * 0.16187844487943592 + even3 * 0.02946017143111912
  c3 = -odd1 * 0.16471626390554542 - odd2 * 0.00154547203542499 + odd3 * 0.03399271444851909
  c4 = even1 * 0.03845798729588149 - even2 * 0.05712936104242644 + even3 * 0.01866750929921070
  c5 = odd1 * 0.04317950185225609 - odd2 * 0.01802814255926417 + odd3 * 0.00152170021558204

  z = np.array((-1./2, 0), dtype = np.float32)[None,None,:]
  
  return (((((c5 * z + c4) * z + c3) * z + c2) * z + c1) * z + c0).reshape(2, -1)

plot_me(oversample2_6point_5_order(signal), 1, 2, MySampleRate = 2 * SampleRate, NFFT = 8192 * 2, noverlap = 1024 * 2)

pyplot.show()
