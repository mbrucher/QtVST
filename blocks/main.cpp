/**
 * \file main.cpp
 */

#include "variable_filter.h"

const unsigned long size = 10000000;
float in[size];
float out[size];

int main(int argc, char** argv)
{
  VariableFilter<float> filter;

  filter.set_cutoff_frequency(48000);
  filter.set_attenuation(1);
  filter.set_cutoff_frequency(12000);

  in[0] = 1;

  filter.process(in, out, size);

  return 0;
}
