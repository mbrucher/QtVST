/**
 * \filter decimation_filter.h
 */

#ifndef DECIMATION_FILTER
#define DECIMATION_FILTER

#include <boost/scoped_array.hpp>

#include "config.h"

namespace DSP
{

/**
 * A decimation filter based on a low pass filter
 */
template<class LowPassFilter>
class DecimationFilter
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

  template<class DataTypeIn>
  void process(const DataTypeIn* RESTRICT in, DataType* RESTRICT out, long size)
  {
    if (size > temp_size)
    {
      temp_array.reset(new DataType[size]);
      temp_size = size;
    }

    filter.process(in, temp_array.get(), size);

    for(int i = 0; i < size / 2; ++i)
    {
      out[i] = temp_array[2 * i];
    }
  }
};

}

#endif
