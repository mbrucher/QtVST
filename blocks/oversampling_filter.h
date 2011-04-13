/**
 * \filter oversampling_filter.h
 */

#ifndef OVERSAMPLING_FILTER
#define OVERSAMPLING_FILTER

#include "config.h"

namespace DSP
{

template<class DataType>
class OversamplingFilter
{
  DataType buffer[6];

public:
  OversamplingFilter()
  {
    for(int i = 0; i < 6; ++i)
    {
      buffer[i] = 0;
    }
  }

  template<class DataTypeIn>
  void process(const DataTypeIn* RESTRICT in, DataType* RESTRICT out, long size)
  {
    for(int i = 0; i < size; ++i)
    {
      for(int j = 0; j < 5; ++j)
      {
        buffer[j] = buffer[j + 1];
      }
      buffer[5] = in[i];

      DataType even1 = buffer[2] +  buffer[3];
      DataType even2 = buffer[1] + buffer[4];
      DataType even3 = buffer[0] + buffer[5];
      DataType odd1 = buffer[3] - buffer[2];
      DataType odd2 = buffer[4] - buffer[1];
      DataType odd3 = buffer[5] - buffer[0];

      DataType c0 = even1 * 0.40513396007145713 + even2 * 0.09251794438424393 + even3 * 0.00234806603570670;
      DataType c1 = odd1 * 0.28342806338906690 + odd2 * 0.21703277024054901 + odd3 * 0.01309294748731515;
      DataType c2 = -even1 * 0.191337682540351941 + even2 * 0.16187844487943592 + even3 * 0.02946017143111912;
      DataType c3 = -odd1 * 0.16471626390554542 - odd2 * 0.00154547203542499 + odd3 * 0.03399271444851909;
      DataType c4 = even1 * 0.03845798729588149 - even2 * 0.05712936104242644 + even3 * 0.01866750929921070;
      DataType c5 = odd1 * 0.04317950185225609 - odd2 * 0.01802814255926417 + odd3 * 0.00152170021558204;

      out[2 * i] = (-(-(-(-(- c5 / 2 + c4) / 2 + c3) / 2 + c2) / 2 + c1) / 2 + c0);
      out[2 * i + 1] = c0;
    }
  }
};

}

#endif
