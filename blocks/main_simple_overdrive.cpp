/**
 * \file main.cpp
 */

#include <fstream>

#include <boost/math/constants/constants.hpp>

#include "newton_raphson_optimizer.h"
#include "simple_overdrive.h"
#include "oversampling_filter.h"

const unsigned long size = 400000;
const unsigned int sample_rate = 96000;
const unsigned int max_frequency = 2000;
double in[size];
double out[size];

int main(int argc, char** argv)
{
  DSP::OversamplingFilter<double> oversampling_filter;

  DSP::SimpleOverdrive<double> overdrive(1./48000, 10000, 22e-9, 1e-12, 26e-3);
  DSP::NewtonRaphsonOptimizer<DSP::SimpleOverdrive<double> > filter(overdrive);

  for(int i = 0; i < size; ++i)
  {
    double j = static_cast<double>(i) / sample_rate;
    in[i] = 20 * std::sin(boost::math::constants::pi<double>() * ((j + .1) * sample_rate * max_frequency / size) * j);
  }

  filter.process(in, out, size);

  std::ofstream infile("in_overdrive.raw", std::ofstream::binary);
  infile.write(reinterpret_cast<const char*>(in), size * sizeof(double));

  std::ofstream outfile("out_overdrive.raw", std::ofstream::binary);
  outfile.write(reinterpret_cast<const char*>(out), size * sizeof(double));
  
  return 0;
}
