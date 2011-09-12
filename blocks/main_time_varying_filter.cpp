/**
 * \file main_time_varying_filter.cpp
 */

#include <fstream>

#include <boost/scoped_ptr.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>

#include "second_order_filter.h"
#include "time_varying_second_order_filter.h"

const unsigned long size = 2000000;
const unsigned int sample_rate = 96000;
const unsigned int max_frequency = 20000;
double in[size];
double temp[size];
double out[size];

DSP::MonoFilter<double>* create_low_wah()
{
  DSP::TimeVaryingSecondOrderFilter<DSP::TimeVaryingBandPassCoefficients<double> >* filter = new DSP::TimeVaryingSecondOrderFilter<DSP::TimeVaryingBandPassCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency_min(100);
  filter->set_cut_frequency_max(10000);
  filter->set_angle_offset(0);
  filter->set_period(sample_rate*4);
  filter->set_Q(1);

  return filter;
}

DSP::MonoFilter<double>* create_high_wah()
{
  DSP::TimeVaryingSecondOrderFilter<DSP::TimeVaryingBandPassCoefficients<double> >* filter = new DSP::TimeVaryingSecondOrderFilter<DSP::TimeVaryingBandPassCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency_min(8000);
  filter->set_cut_frequency_max(10000);
  filter->set_angle_offset(boost::math::constants::pi<double>() / 2);
  filter->set_period(sample_rate*8);
  filter->set_Q(1);

  return filter;
}

int main(int argc, char** argv)
{
  boost::scoped_ptr<DSP::MonoFilter<double> > low_wah_filter(create_low_wah());
  boost::scoped_ptr<DSP::MonoFilter<double> > high_wah_filter(create_high_wah());

  boost::mt19937 engine(0);
  boost::normal_distribution<double> generator;
  boost::variate_generator<boost::mt19937, boost::normal_distribution<double> > binded(engine, generator);

  for(int i = 0; i < size; ++i)
  {
    in[i] = binded();
  }

  low_wah_filter->process(in, out, size);
  high_wah_filter->process(in, temp, size);

  for(int i = 0; i < size; ++i)
  {
    out[i] += temp[i];
  }

  std::ofstream infile("in_time_varying.raw", std::ofstream::binary);
  infile.write(reinterpret_cast<const char*>(in), size * sizeof(double));

  std::ofstream outfile("out_time_varying.raw", std::ofstream::binary);
  outfile.write(reinterpret_cast<const char*>(out), size * sizeof(double));

  return 0;
}
