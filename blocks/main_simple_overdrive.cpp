/**
 * \file main.cpp
 */

#include <fstream>

#include <boost/math/constants/constants.hpp>

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

  for(int i = 0; i < size; ++i)
  {
    in[i] = 20 * std::sin(2 * boost::math::constants::pi<float>() * i * 100 / 48000);
  }

  //overdrive.process(in, out, size);

  std::ofstream outfile("out_overdrive.raw");
  outfile.write(reinterpret_cast<const char*>(out), size * sizeof(float));
  
  return 0;
}
