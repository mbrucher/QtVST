/**
 * \filter decimation_filter.h
 */

#ifndef DSP_DECIMATION_FILTER
#define DSP_DECIMATION_FILTER

#include <boost/scoped_array.hpp>

#include "filter.h"

namespace DSP
{

/**
 * A decimation filter based on a low pass filter
 */
template<int DecimationFactor, class LowPassFilter>
class DecimationFilter: public MonoFilter<typename LowPassFilter::DataType>
{
public:
  typedef typename LowPassFilter::DataType DataType;
private:
  LowPassFilter filter;

  boost::scoped_array<DataType> temp_array;
  int temp_size;
public:
  DecimationFilter()
  :temp_size(0)
  {
  }

  /**
   * Returns the inner filter to modify its inner parameters
   */
  LowPassFilter& get_filter()
  {
    return filter;
  }

  DSP_MONOFILTER_DECLARE()

  template<class DataTypeIn, class DataTypeOut>
  void process(const DataTypeIn* RESTRICT in, DataTypeOut* RESTRICT out, unsigned long size)
  {
    if (size > temp_size)
    {
      temp_array.reset(new DataType[size]);
      temp_size = size;
    }

    filter.process(in, temp_array.get(), size);

    for(int i = 0; i < size / DecimationFactor; ++i)
    {
      out[i] = temp_array[DecimationFactor * i];
    }
  }
};

}

#endif
