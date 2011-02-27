/**
 * \file main.cpp
 */

#include <fstream>

#include <boost/math/constants/constants.hpp>

#include "newton_raphson_optimizer.h"
#include "simple_overdrive.h"

const unsigned long size = 10000000;
float in[size];
float out[size];

int main(int argc, char** argv)
{
  DSP::SimpleOverdrive<float> overdrive(1./48000, 10000, 22e-9, 1e-12, 26e-3);
  DSP::NewtonRaphsonOptimizer<DSP::SimpleOverdrive<float> > filter;

  for(int i = 0; i < size; ++i)
  {
    in[i] = 20 * std::sin(2 * boost::math::constants::pi<float>() * i * 100 / 48000);
  }

  filter.process(in, out, size);

  std::ofstream outfile("out_overdrive.raw");
  outfile.write(reinterpret_cast<const char*>(out), size * sizeof(float));
  
  return 0;
}
