/**
 * \file main.cpp
 */

#include <fstream>

#include <boost/math/constants/constants.hpp>

#include "newton_raphson_optimizer.h"
#include "simple_overdrive.h"

const unsigned long size = 40000;
const unsigned int sample_rate = 96000;
const unsigned int max_frequency = 2000;
float in[size];
float out[size];

int main(int argc, char** argv)
{
  DSP::SimpleOverdrive<float> overdrive(1./48000, 10000, 22e-9, 1e-12, 26e-3);
  DSP::NewtonRaphsonOptimizer<DSP::SimpleOverdrive<float> > filter(overdrive);

  for(int i = 0; i < size; ++i)
  {
    float j = static_cast<float>(i) / sample_rate;
    in[i] = 20 * std::sin(boost::math::constants::pi<float>() * ((j + .1) * sample_rate * max_frequency / size) * j);
  }

  filter.process(in, out, size);

  std::ofstream infile("in_overdrive.raw", std::ofstream::binary);
  infile.write(reinterpret_cast<const char*>(in), size * sizeof(float));

  std::ofstream outfile("out_overdrive.raw", std::ofstream::binary);
  outfile.write(reinterpret_cast<const char*>(out), size * sizeof(float));
  
  return 0;
}
