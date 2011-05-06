/**
 * \file main.cpp
 */

#include <fstream>

#include <boost/scoped_ptr.hpp>
#include <boost/math/constants/constants.hpp>

#include "first_order_filter.h"
#include "second_order_filter.h"

const unsigned long size = 2000000;
const unsigned int sample_rate = 96000;
const unsigned int max_frequency = 20000;
double in[size];
double temp[size];
double out[size];

DSP::MonoFilter<double>* create_low_shelving()
{
  DSP::SecondOrderFilter<DSP::LowShelvingCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::LowShelvingCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(100);
  filter->set_gain(.1);

  return filter;
}

DSP::MonoFilter<double>* create_low_peak()
{
  DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(1000);
  filter->set_gain(10);
  filter->set_Q(10);

  return filter;
}

DSP::MonoFilter<double>* create_high_peak()
{
  DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(10000);
  filter->set_gain(10);
  filter->set_Q(10);

  return filter;
}

DSP::MonoFilter<double>* create_high_shelving()
{
  DSP::SecondOrderFilter<DSP::HighShelvingCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::HighShelvingCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(15000);
  filter->set_gain(.1);

  return filter;
}

int main(int argc, char** argv)
{
  boost::scoped_ptr<DSP::MonoFilter<double> > low_shelving_filter(create_low_shelving());
  boost::scoped_ptr<DSP::MonoFilter<double> > low_peak_filter(create_low_peak());
  boost::scoped_ptr<DSP::MonoFilter<double> > high_peak_filter(create_high_peak());
  boost::scoped_ptr<DSP::MonoFilter<double> > high_shelving_filter(create_high_shelving());

  for(int i = 0; i < size; ++i)
  {
    double j = static_cast<double>(i) / sample_rate;
    in[i] = std::sin(boost::math::constants::pi<double>() * ((j + .1) * sample_rate * max_frequency / size) * j);
  }

  low_shelving_filter->process(in, temp, size);
  low_peak_filter->process(temp, out, size);
  high_peak_filter->process(out, temp, size);
  high_shelving_filter->process(temp, out, size);

  std::ofstream infile("in_eq.raw", std::ofstream::binary);
  infile.write(reinterpret_cast<const char*>(in), size * sizeof(double));

  std::ofstream outfile("out_eq.raw", std::ofstream::binary);
  outfile.write(reinterpret_cast<const char*>(out), size * sizeof(double));

  return 0;
}
