#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt

import oversampling
import simple_overdrive

SampleRate = 48000
Samples = 1000000
FreqMax = 20000

t = np.arange(Samples, dtype=np.float64) / SampleRate
input1 = np.sin(np.pi * (SampleRate * FreqMax / Samples * (t + .1)) * t)
input2 = input1[::-1].flatten()

signal = np.array((input1, input2))

print "Oversampling"
oversampled_signal = oversampling.oversample2_6point_5_order(signal)

oversampling.plot_me(signal, 0, 2)
print "Overdrive"
overdriven_signal = (simple_overdrive.overdrive(signal[0], fs = SampleRate), simple_overdrive.overdrive(signal[1], fs = SampleRate))
oversampling.plot_me(overdriven_signal, 1, 2)

plt.figure()
oversampling.plot_me(signal, 0, 2)
print "Overdrive oversampling"
overdriven_oversampled_signal = (simple_overdrive.overdrive(oversampled_signal[0], fs = SampleRate * 2), simple_overdrive.overdrive(oversampled_signal[1], fs = SampleRate * 2))
oversampling.plot_me(overdriven_oversampled_signal, 1, 2, SampleRate = SampleRate * 2)

plt.show()
