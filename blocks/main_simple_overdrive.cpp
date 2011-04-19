/**
 * \file main.cpp
 */

#include <fstream>

#include <boost/scoped_ptr.hpp>
#include <boost/math/constants/constants.hpp>

#include "newton_raphson_optimizer.h"
#include "simple_overdrive.h"
#include "oversampling_filter.h"
#include "gain_filter.h"
#include "first_order_filter.h"
#include "second_order_filter.h"
#include "decimation_filter.h"

const unsigned long size = 2000000;
const unsigned int sample_rate = 96000;
const unsigned int max_frequency = 20000;
double in[size];
double gain[size];
double out[size];

const int oversampling = 8;

double in_oversampled[oversampling * size];
double out_oversampled[oversampling * size];

DSP::MonoFilter<double>* create_gain()
{
  DSP::GainFilter<double>* gain_filter = new DSP::GainFilter<double>;
  gain_filter->set_gain(20);
  return gain_filter;
}

DSP::MonoFilter<double>* create_oversampling_filter()
{
  DSP::OversamplingFilter<oversampling, DSP::Oversampling6points5order<double>, double>* oversampling_filter = new DSP::OversamplingFilter<oversampling, DSP::Oversampling6points5order<double>, double>;

  return oversampling_filter;
}

DSP::MonoFilter<double>* create_overdrive()
{
  DSP::NewtonRaphsonOptimizer<DSP::SimpleOverdrive<double> >* filter = new DSP::NewtonRaphsonOptimizer<DSP::SimpleOverdrive<double> >(DSP::SimpleOverdrive<double>(1./sample_rate / oversampling, 10000, 22e-9, 1e-12, 26e-3));

  return filter;
}

DSP::MonoFilter<double>* create_low_filter()
{
  DSP::SecondOrderFilter<DSP::LowPassCoefficients<double>, double>* low_filter = new DSP::SecondOrderFilter<DSP::LowPassCoefficients<double>, double>;

  low_filter->set_sampling_frequency(sample_rate * oversampling);
  low_filter->set_cut_frequency(max_frequency);

  return low_filter;
}

DSP::MonoFilter<double>* create_decimation_low_filter()
{
  DSP::DecimationFilter<oversampling, DSP::SecondOrderFilter<DSP::LowPassCoefficients<double>, double>, double>* decimation_low_filter = new DSP::DecimationFilter<oversampling, DSP::SecondOrderFilter<DSP::LowPassCoefficients<double>, double>, double>;

  decimation_low_filter->get_filter().set_sampling_frequency(sample_rate * oversampling);
  decimation_low_filter->get_filter().set_cut_frequency(max_frequency);

  return decimation_low_filter;
}

int main(int argc, char** argv)
{
  boost::scoped_ptr<DSP::MonoFilter<double> > gain_filter(create_gain());
  boost::scoped_ptr<DSP::MonoFilter<double> > oversampling_filter(create_oversampling_filter());
  boost::scoped_ptr<DSP::MonoFilter<double> > filter(create_overdrive());
  boost::scoped_ptr<DSP::MonoFilter<double> > low_filter(create_low_filter());
  boost::scoped_ptr<DSP::MonoFilter<double> > decimation_low_filter(create_decimation_low_filter());

  for(int i = 0; i < size; ++i)
  {
    double j = static_cast<double>(i) / sample_rate;
    in[i] = std::sin(boost::math::constants::pi<double>() * ((j + .1) * sample_rate * max_frequency / size) * j);
  }

  gain_filter->process(in, gain, size);
  oversampling_filter->process(gain, in_oversampled, size);
  filter->process(in_oversampled, out_oversampled, oversampling * size);
  low_filter->process(out_oversampled, in_oversampled, oversampling * size);
  decimation_low_filter->process(in_oversampled, out, oversampling * size);

  std::ofstream infile("in_overdrive.raw", std::ofstream::binary);
  infile.write(reinterpret_cast<const char*>(in), size * sizeof(double));

  std::ofstream outfile("out_overdrive.raw", std::ofstream::binary);
  outfile.write(reinterpret_cast<const char*>(out), size * sizeof(double));

  return 0;
}
