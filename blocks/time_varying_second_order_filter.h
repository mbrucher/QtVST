/**
 * \filter time_varying_second_order_filter.h
 */

#ifndef DSP_TIME_VARYING_SECOND_ORDER_FILTER
#define DSP_TIME_VARYING_SECOND_ORDER_FILTER

#include <boost/math/constants/constants.hpp>
#include <boost/scoped_array.hpp>

#include "filter.h"

namespace DSP
{
template<class Data_Type>
class TimeVaryingBandPassCoefficients
{
public:
  typedef Data_Type DataType;
private:
  DataType sampling_frequency;
  DataType cut_frequency_min;
  DataType cut_frequency_max;
  DataType angle_offset;
  DataType Q;

  void compute_coeffs()
  {
    coefficients_in.reset(new DataType[period][3]);
    coefficients_out.reset(new DataType[period][2]);
    offset = 0;

    for(int i = 0; i < period; ++i)
    {
      DataType c = std::tan(boost::math::constants::pi<DataType>() * (cut_frequency_min + (cut_frequency_max - cut_frequency_min) * (std::cos(angle_offset + i * 2 * boost::math::constants::pi<DataType>() / period) + 1) / 2) / sampling_frequency);
      DataType d = (1 + std::sqrt(2.) * c + c * c);
      DataType Q_inv = 1 / Q;

      coefficients_in[i][2] = Q_inv * c / d;
      coefficients_in[i][1] = 0;
      coefficients_in[i][0] = -Q_inv * c / d;
      coefficients_out[i][1] = - 2 * (c * c - 1) / d;
      coefficients_out[i][0] = - (1 - std::sqrt(2.) * c + c * c) / d;
    }
  }

protected:
  boost::scoped_array<DataType[3]> coefficients_in;
  boost::scoped_array<DataType[2]> coefficients_out;
  int offset;
  int period;

public:
  void set_sampling_frequency(DataType sampling_frequency)
  {
    this->sampling_frequency = sampling_frequency;
    compute_coeffs();
  }

  void set_cut_frequency_min(DataType cut_frequency_min)
  {
    this->cut_frequency_min = cut_frequency_min;
    compute_coeffs();
  }

  void set_cut_frequency_max(DataType cut_frequency_max)
  {
    this->cut_frequency_max = cut_frequency_max;
    compute_coeffs();
  }

  void set_period(int period)
  {
    this->period = period;
    compute_coeffs();
  }

  void set_angle_offset(DataType angle_offset)
  {
    this->angle_offset = angle_offset;
    compute_coeffs();
  }

  void set_Q(DataType Q)
  {
    this->Q = Q;
    compute_coeffs();
  }
};

template<class Coefficients>
class TimeVaryingSecondOrderFilter: public Coefficients, public MonoFilter<typename Coefficients::DataType>
{
  typedef typename Coefficients::DataType DataType;
  DataType buffer_in[2];
  DataType buffer_out[2];

  using Coefficients::coefficients_in;
  using Coefficients::coefficients_out;
  using Coefficients::period;
  using Coefficients::offset;

public:
  TimeVaryingSecondOrderFilter()
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
      out[i] = coefficients_in[offset][2] * in[i] + coefficients_in[offset][1] * buffer_in[1] + coefficients_in[offset][0] * buffer_in[0] + coefficients_out[offset][1] * buffer_out[1] + coefficients_out[offset][0] * buffer_out[0];

      buffer_in[0] = buffer_in[1];
      buffer_in[1] = in[i];
      buffer_out[0] = buffer_out[1];
      buffer_out[1] = out[i];

      offset++;
      if(offset >= period)
      {
        offset = 0;
      }
    }
  }

};

}

#endif
