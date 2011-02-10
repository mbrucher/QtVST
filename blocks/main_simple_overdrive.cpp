/**
 * \file main.cpp
 */

#include "simple_overdrive.h"

const unsigned long size = 10000000;
float in[size];
float out[size];

int main(int argc, char** argv)
{
  DSP::SimpleOverdrive<float> overdrive(1./48000, 10000, 22e-9, 1e-12, 26e-3);
  
  //overdrive.set_cutoff_frequency(48000);
  //overdrive.set_attenuation(1);
  //overdrive.set_cutoff_frequency(12000);

  in[0] = 1;

  //overdrive.process(in, out, size);

  return 0;
}
