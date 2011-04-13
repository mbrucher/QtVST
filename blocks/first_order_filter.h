/**
 * \filter first_order_filter.h
 */

#ifndef FIRST_ORDER_FILTER
#define FIRST_ORDER_FILTER

#include <boost/math/constants/constants.hpp>

#include "config.h"

namespace DSP
{

template<class DataType>
class AllPassFilter
{
  DataType buffer_in[2];
  DataType buffer_out[1];

  DataType sampling_frequency;
  DataType cut_frequency;
  DataType c;

  void compute_coeffs()
  {
    c = (std::tan(boost::math::constants::pi<DataType>() * cut_frequency / sampling_frequency) - 1) / (std::tan(boost::math::constants::pi<DataType>() * cut_frequency / sampling_frequency) + 1);
  }

public:
  AllPassFilter()
  {
    for(int i = 0; i < 2; ++i)
    {
      buffer_in[i] = 0;
    }
    buffer_out[0] = 0;
  }

  void set_sampling_frequency(DataType sampling_frequency)
  {
    this->sampling_frequency = sampling_frequency;
    compute_coeffs();
  }

  void set_cut_frequency(DataType cut_frequency)
  {
    this->cut_frequency = cut_frequency;
    compute_coeffs();
  }

  template<class DataTypeIn>
  void process(const DataTypeIn* RESTRICT in, DataType* RESTRICT out, long size)
  {
    for(int i = 0; i < size; ++i)
    {
      buffer_in[0] = buffer_in[1];
      buffer_in[1] = in[i];

      out[i] = c * buffer_in[0] + buffer_in[1] - c * buffer_out[0];
      buffer_out[0] = out[i];
    }
  }
};

template<class DataType>
class LowPassFilter
{
  AllPassFilter<DataType> all_pass_filter;

public:
  void set_sampling_frequency(DataType sampling_frequency)
  {
    all_pass_filter.set_sampling_frequency(sampling_frequency);
  }

  void set_cut_frequency(DataType cut_frequency)
  {
    all_pass_filter.set_cut_frequency(cut_frequency);
  }

  template<class DataTypeIn>
  void process(const DataTypeIn* RESTRICT in, DataType* RESTRICT out, long size)
  {
    all_pass_filter.process(in, out, size);

    for(int i = 0; i < size; ++i)
    {
      out[i] = (out[i] + in[i]) / 2;
    }
  }
};


template<class DataType>
class HighPassFilter
{
  AllPassFilter<DataType> all_pass_filter;

public:
  void set_sampling_frequency(DataType sampling_frequency)
  {
    all_pass_filter.set_sampling_frequency(sampling_frequency);
  }

  void set_cut_frequency(DataType cut_frequency)
  {
    all_pass_filter.set_cut_frequency(cut_frequency);
  }

  template<class DataTypeIn>
  void process(const DataTypeIn* RESTRICT in, DataType* RESTRICT out, long size)
  {
    all_pass_filter.process(in, out, size);

    for(int i = 0; i < size; ++i)
    {
      out[i] = (out[i] - in[i]) / 2;
    }
  }
};
}

#endif
