/**
 * \filter butterworth_filter.h
 */

#ifndef DSP_BUTTERWORTH_FILTER
#define DSP_BUTTERWORTH_FILTER

#include <boost/math/special_functions/binomial.hpp>
#include <boost/math/constants/constants.hpp>

#include "filter.h"

namespace DSP
{

/**
 * A first order allpass filter
 */
template<class Data_Type, int Order>
class ButterworthFilter: public MonoFilter<Data_Type>
{
public:
  typedef Data_Type DataType;
private:
  DataType buffer_in[Order+1];
  DataType buffer_out[Order+1];

  DataType sampling_frequency;
  DataType cut_frequency;
  DataType c_in[Order+1];
  DataType c_out[Order+1];

  void compute_coeffs()
  {
    DataType coeff = std::pow(sampling_frequency / (boost::math::constants::pi<DataType>() * cut_frequency), Order);
    for (int i = 0; i < Order+1; ++i)
    {
      c_in[i] = boost::math::binomial_coefficient<DataType>(Order, i);
      c_out[i] = i==Order?1:0;
    }
  }

public:
  ButterworthFilter()
  {
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

  DSP_MONOFILTER_DECLARE()

  template<class DataTypeIn, class DataTypeOut>
  void process(const DataTypeIn* RESTRICT in, DataTypeOut* RESTRICT out, unsigned long size)
  {
    for(int i = 0; i < size; ++i)
    {
      for(int j = 0; j < Order; ++j)
      {
        buffer_in[j] = buffer_in[j+1];
        buffer_out[j] = buffer_out[j+1];
      }
      buffer_in[Order] = in[i];
      DataType xh = 0;

      for(int j = 0; j < Order+1; ++j)
      {
        xh += c_in[j] * buffer_in[j];
      }
      for(int j = 0; j < Order; ++j)
      {
        xh -= c_out[j] * buffer_out[j];
      }

      out[i] = xh / c_out[Order];
      buffer_out[Order] = out[i];
    }
  }
};
}

#endif
