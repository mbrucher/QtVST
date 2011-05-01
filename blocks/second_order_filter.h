/**
 * \filter second_order_filter.h
 */

#ifndef DSP_SECOND_ORDER_FILTER
#define DSP_SECOND_ORDER_FILTER

#include <boost/math/constants/constants.hpp>

#include "filter.h"

namespace DSP
{
/**
 * Coefficients for a second order allpass filter
 */
template<class Data_Type>
class AllPassCoefficients
{
public:
  typedef Data_Type DataType;
private:
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

/**
 * Coefficients for a second order lowpass filter
 */
template<class Data_Type>
class LowPassCoefficients
{
public:
  typedef Data_Type DataType;
private:
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

/**
 * Coefficients for a second order highpass filter
 */
template<class Data_Type>
class HighPassCoefficients
{
public:
  typedef Data_Type DataType;
private:
  DataType sampling_frequency;
  DataType cut_frequency;

  void compute_coeffs()
  {
    DataType c = std::tan(boost::math::constants::pi<DataType>() * cut_frequency / sampling_frequency);
    DataType d = (1 + std::sqrt(2.) * c + c * c);

    coefficients_in[2] = 1;
    coefficients_in[1] = -2;
    coefficients_in[0] = 1;
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

/**
 * Second order filter template class
 */
template<class Coefficients>
class SecondOrderFilter: public Coefficients, public MonoFilter<typename Coefficients::DataType>
{
  typedef typename Coefficients::DataType DataType;
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

/**
 * Coefficients for a second order lowpass filter
 */
template<class Data_Type>
class LowPassShelvingCoefficients
{
public:
  typedef Data_Type DataType;
private:
  DataType sampling_frequency;
  DataType cut_frequency;

  void compute_coeffs()
  {
    DataType c = std::tan(boost::math::constants::pi<DataType>() * cut_frequency / sampling_frequency);
    if(gain <= 1)
    {
      DataType V0 = 1 / gain;
      DataType d = (1 + std::sqrt(2. * V0) * c + V0 * c * c);

      coefficients_in[2] = (1 + std::sqrt(2.) * c + c * c) / d;
      coefficients_in[1] = 2 * (c * c - 1) / d;
      coefficients_in[0] = (1 - std::sqrt(2.) * c + c * c) / d;
      coefficients_out[1] = - 2 * (V0 * c * c - 1) / d;
      coefficients_out[0] = - (1 - std::sqrt(2. * V0) * c + V0 * c * c) / d;
    }
    else
    {
      DataType d = (1 + std::sqrt(2.) * c + c * c);

      coefficients_in[2] = (1 + std::sqrt(2. * gain) * c + gain * c * c) / d;
      coefficients_in[1] = 2 * (gain * c * c - 1) / d;
      coefficients_in[0] = (1 - std::sqrt(2. * gain) * c + gain * c * c) / d;
      coefficients_out[1] = - 2 * (c * c - 1) / d;
      coefficients_out[0] = - (1 - std::sqrt(2.) * c + c * c) / d;
    }
  }

protected:
  DataType coefficients_in[3];
  DataType coefficients_out[2];
  DataType gain;

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

  void set_gain(DataType gain)
  {
    this->gain = gain;
    compute_coeffs();
  }
};

/**
 * Coefficients for a second order high pass shelving filter
 */
template<class Data_Type>
class HighPassShelvingCoefficients
{
public:
  typedef Data_Type DataType;
private:
  DataType sampling_frequency;
  DataType cut_frequency;

  void compute_coeffs()
  {
    DataType c = std::tan(boost::math::constants::pi<DataType>() * cut_frequency / sampling_frequency);
    if(gain <= 1)
    {
      DataType V0 = 1 / gain;
      DataType d = (V0 + std::sqrt(2. * V0) * c + c * c);

      coefficients_in[2] = -(1 + std::sqrt(2.) * c + c * c) / d;
      coefficients_in[1] = -2 * (c * c - 1) / d;
      coefficients_in[0] = -(1 - std::sqrt(2.) * c + c * c) / d;
      coefficients_out[1] = - 2 * (c * c - V0) / d;
      coefficients_out[0] = - (V0 - std::sqrt(2. * V0) * c + c * c) / d;
    }
    else
    {
      DataType d = (1 + std::sqrt(2.) * c + c * c);

      coefficients_in[2] = -(gain + std::sqrt(2. * gain) * c + c * c) / d;
      coefficients_in[1] = -2 * (c * c - gain) / d;
      coefficients_in[0] = -(gain - std::sqrt(2. * gain) * c + c * c) / d;
      coefficients_out[1] = - 2 * (c * c - 1) / d;
      coefficients_out[0] = - (1 - std::sqrt(2.) * c + c * c) / d;
    }
  }

protected:
  DataType coefficients_in[3];
  DataType coefficients_out[2];
  DataType gain;

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

  void set_gain(DataType gain)
  {
    this->gain = gain;
    compute_coeffs();
  }
};

/**
 * Second order shelving filter template class
 */
template<class Coefficients>
class SecondOrderShelvingFilter: public Coefficients, public MonoFilter<typename Coefficients::DataType>
{
  typedef typename Coefficients::DataType DataType;
  DataType buffer_in[2];
  DataType buffer_out[2];

  using Coefficients::coefficients_in;
  using Coefficients::coefficients_out;
  using Coefficients::gain;
public:
  SecondOrderShelvingFilter()
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
      
      //out[i] = (gain - 1) * (in[i] + out[i]) / 2 + in[i];
    }
  }

};
}

#endif
