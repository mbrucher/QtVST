/**
 * \filter butterworth_filter.h
 */

#ifndef DSP_BUTTERWORTH_FILTER
#define DSP_BUTTERWORTH_FILTER

#include <boost/math/special_functions/binomial.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/polynomial.hpp>

#include "filter.h"

namespace DSP
{

/**
 * A Butterworth lowpass filter
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
    DataType coeff = sampling_frequency / (boost::math::constants::pi<DataType>() * cut_frequency);
    DataType temp1[2] = {1,-1};
    DataType temp2[2] = {1,+1};
    boost::math::tools::polynomial<DataType> poly1(temp1, 1);
    boost::math::tools::polynomial<DataType> poly2(temp2, 1);

    for (int i = 0; i < Order+1; ++i)
    {
      c_in[i] = boost::math::binomial_coefficient<DataType>(Order, i);
    }
	
    boost::math::tools::polynomial<DataType> poly(1);
	
    if(Order % 2 != 0)
    {
      poly = poly2 + coeff * poly1;
    }

    for(int i = 1; i <= Order/2; ++i)
    {
      boost::math::tools::polynomial<DataType> polytemp = coeff * coeff * poly1 * poly1;
      polytemp -= 2 * coeff * std::cos((2. * i + Order - 1) / (2 * Order) * boost::math::constants::pi<DataType>()) * poly1 * poly2;
      polytemp += poly2 * poly2;
      
      poly *= polytemp;
    }

    for (int i = 0; i < poly.size(); ++i)
    {
      c_out[Order - i] = poly[i] / poly[0];
    }
    for (int i = 0; i < Order+1; ++i)
    {
      c_in[i] /= poly[0];
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

      out[i] = xh;
      buffer_out[Order] = out[i];
    }
  }
};
}

#endif
