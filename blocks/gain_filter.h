/**
 * \file gain_filter.hpp
 */

#ifndef DSP_GAINFILTER
#define DSP_GAINFILTER

#include "filter.h"

namespace DSP
{

/**
 * Gain filter
 */
template<class Data_Type>
class GainFilter: public MonoFilter<Data_Type>
{
public:
  typedef Data_Type DataType;
private:
  DataType gain;

public:
  GainFilter()
    :gain(0)
  {
  }

  DSP_MONOFILTER_DECLARE()

  template<class DataTypeIn>
  void process(const DataTypeIn* in, DataType* out, unsigned long nb_samples)
  {
    for(unsigned long i = 0; i < nb_samples; ++i)
    {
	  out[i] = in[i] * gain;
    }
  } 

  void set_gain(DataType gain)
  {
    this->gain = gain;
  }
};

}
#endif
