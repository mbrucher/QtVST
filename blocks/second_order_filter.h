/**
 * \filter second_order_filter.h
 */

#ifndef DSP_SECOND_ORDER_FILTER
#define DSP_SECOND_ORDER_FILTER

#include <boost/math/constants/constants.hpp>

#include "filter.h"

namespace DSP
{
template<class DataType>
class AllPassCoefficients
{
  DataType sampling_frequency;
  DataType cut_frequency;
  DataType bandwidth_frequency;

  void compute_coeffs()
  {
    DataType c = (std::tan(boost::math::constants::pi<DataType>() * bandwidth_frequency / sampling_frequency) - 1) / (std::tan(boost::math::constants::pi<DataType>() * bandwidth_frequency / sampling_frequency) + 1);
    DataType d = - std::cos(2 * boost::math::constants::pi<DataType>() * cut_frequency / sampling_frequency);

    coefficients_in[2] = -c;
    coefficients_in[1] = d * (1 - c);
    coefficients_in[0] = 1;
    coefficients_out[1] = - d * (1 - c);
    coefficients_out[0] = c;
  }

protected:
  DataType coefficients_in[3];
  DataType coefficients_out[2];

public:
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

  void set_bandwidth_frequency(DataType bandwidth_frequency)
  {
    this->bandwidth_frequency = bandwidth_frequency;
    compute_coeffs();
  }
};

template<class DataType>
class LowPassCoefficients
{
  DataType sampling_frequency;
  DataType cut_frequency;

  void compute_coeffs()
  {
    DataType c = std::tan(boost::math::constants::pi<DataType>() * cut_frequency / sampling_frequency);
    DataType d = (1 + std::sqrt(2.) * c + c * c);

    coefficients_in[2] = c * c / d;
    coefficients_in[1] = 2 * c * c / d;
    coefficients_in[0] = c * c / d;
    coefficients_out[1] = - 2 * (c * c - 1) / d;
    coefficients_out[0] = - (1 - std::sqrt(2.) * c + c * c) / d;
  }

protected:
  DataType coefficients_in[3];
  DataType coefficients_out[2];

public:
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
};

template<class Coefficients, class DataType>
class SecondOrderFilter: public Coefficients, public MonoFilter<DataType>
{
  DataType buffer_in[2];
  DataType buffer_out[2];

  using Coefficients::coefficients_in;
  using Coefficients::coefficients_out;

public:
  SecondOrderFilter()
  :Coefficients()
  {
    for(int i = 0; i < 2; ++i)
    {
      buffer_in[i] = 0;
      buffer_out[i] = 0;
    }
  }

  DSP_MONOFILTER_DECLARE()

  template<class DataTypeIn, class DataTypeOut>
  void process(const DataTypeIn* RESTRICT in, DataTypeOut* RESTRICT out, unsigned long size)
  {
    for(int i = 0; i < size; ++i)
    {
      out[i] = coefficients_in[2] * in[i] + coefficients_in[1] * buffer_in[1] + coefficients_in[0] * buffer_in[0] + coefficients_out[1] * buffer_out[1] + coefficients_out[0] * buffer_out[0];

      buffer_in[0] = buffer_in[1];
      buffer_in[1] = in[i];
      buffer_out[0] = buffer_out[1];
      buffer_out[1] = out[i];
    }
  }

};

}

#endif
