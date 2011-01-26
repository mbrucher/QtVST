#!/usr/bin/env python

import numpy as np

class Function(object):
  def __init__(self, dt, R, C):
    self.A = dt / (2 * C) + R
    self.B = dt / (2 * C) - R

  def f(self, x):
    return np.sign(x) * 1e-12 * (np.exp(np.abs(x) / 26e-3) - 1)

  def fprime(self, x):
    return 1e-12 * np.exp(np.abs(x) / 26e-3) / 26e-3

  def __call__(self, x0, x1, y0, y1):
    return self.f(y1) + 1/self.A * (y1 + (x0 - x1 + self.B * self.f(y0) - y0))

  def gradient(self, x0, x1, y0, y1):
    return self.fprime(y1) + 1/self.A
    
def optimize(fun, x0, x1, y0):
  y1 = y0
  yk = y1 - fun(x0, x1, y0, y1) / fun.gradient(x0, x1, y0, y1)
  
  while(abs(y1 - yk) > 0.00001):
    y1 = yk
    yk = y1 - fun(x0, x1, y0, y1) / fun.gradient(x0, x1, y0, y1)

  return y1

def plot_fft(x, fs):
  n = len(x)
  x = x[-fs:] / (fs / 2)
  x_fft = fft.fft(x)[:fs/10]
  plt.subplot(211)
  plt.semilogy(np.abs(x_fft))
  plt.subplot(212)
  angle = np.angle(x_fft)
  angle[np.abs(x_fft) < 0.001] = 0
  plt.plot(angle)
  
if __name__ == "__main__":
  import matplotlib.pyplot as plt
  import numpy as np
  import scipy.fftpack as fft
  import scipy.io.wavfile as wav
  
  fs = 44100
  fsin = 200
  n_samples = fs + fs/fsin
  
  fun = Function(1./fs, 10000, 22e-9)
  
  x = 20 * np.sin(2 * np.pi * np.arange(n_samples)*fsin/fs)
  y = np.zeros(n_samples)
  
  x1 = x[0]
  y1 = y[0]
  
  for i in range(1, n_samples):
    x0 = x1
    x1 = x[i]
    y0 = y1
    y1 = optimize(fun, x0, x1, y0)
    y[i] = y1

  plt.plot(x)
  plt.plot(y)

  plt.figure()
  plot_fft(x, fs)

  plt.figure()
  plot_fft(y, fs)
  
  wav.write("sin.wav", fs, (x / 20 * 2**15).astype(np.int16))
  wav.write("sin_overdrive.wav", fs, (y * 2**15).astype(np.int16))
  
  plt.show()
