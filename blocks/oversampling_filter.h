/**
 * \filter oversampling_filter.h
 */

#ifndef OVERSAMPLING_FILTER
#define OVERSAMPLING_FILTER

#include "config.h"

namespace DSP
{

template<class DataType>
class Oversampling6points5order
{
public:
  static const int points = 6;
  static const int order = 5;

protected:
  DataType coeffs[order + 1][points / 2];

public:
  Oversampling6points5order()
  {
    coeffs[0][0] = 0.40513396007145713;
    coeffs[0][1] = 0.09251794438424393;
    coeffs[0][2] = 0.00234806603570670;
    coeffs[1][0] = 0.28342806338906690;
    coeffs[1][1] = 0.21703277024054901;
    coeffs[1][2] = 0.01309294748731515;
    coeffs[2][0] = -0.191337682540351941;
    coeffs[2][1] = 0.16187844487943592;
    coeffs[2][2] = 0.02946017143111912;
    coeffs[3][0] = -0.16471626390554542;
    coeffs[3][1] = -0.00154547203542499;
    coeffs[3][2] = 0.03399271444851909;
    coeffs[4][0] = 0.03845798729588149;
    coeffs[4][1] = -0.05712936104242644;
    coeffs[4][2] = 0.01866750929921070;
    coeffs[5][0] = 0.04317950185225609;
    coeffs[5][1] = -0.01802814255926417;
    coeffs[5][2] = 0.00152170021558204;
  }
};

template<int OversamplingFactor, class OversamplingElements, class DataType>
class OversamplingFilter: public OversamplingElements
{
  DataType buffer[OversamplingElements::points];

  using OversamplingElements::coeffs;

public:
  OversamplingFilter()
  :OversamplingElements()
  {
    for(int i = 0; i < OversamplingElements::points; ++i)
    {
      buffer[i] = 0;
    }
  }

  template<class DataTypeIn>
  void process(const DataTypeIn* RESTRICT in, DataType* RESTRICT out, long size)
  {
    for(int i = 0; i < size; ++i)
    {
      for(int j = 0; j < OversamplingElements::points - 1; ++j)
      {
        buffer[j] = buffer[j + 1];
      }
      buffer[OversamplingElements::points - 1] = in[i];

      DataType even[OversamplingElements::points / 2];
      for(int j = 0; j < OversamplingElements::points / 2; ++j)
      {
        even[j] = buffer[OversamplingElements::points / 2 + j] + buffer[OversamplingElements::points / 2 - 1 - j];
      }
      DataType odd[OversamplingElements::points / 2];
      for(int j = 0; j < OversamplingElements::points / 2; ++j)
      {
        odd[j] = buffer[OversamplingElements::points / 2 + j] - buffer[OversamplingElements::points / 2 - 1 - j];
      }

      DataType c[OversamplingElements::order + 1];

      for(int j = 0; j < OversamplingElements::order + 1; j += 2)
      {
        c[j] = 0;
        for(int k = 0; k < OversamplingElements::points / 2; ++k)
        {
          c[j] += even[k] * coeffs[j][k];
        }
      }
      for(int j = 1; j < OversamplingElements::order + 1; j += 2)
      {
        c[j] = 0;
        for(int k = 0; k < OversamplingElements::points / 2; ++k)
        {
          c[j] += odd[k] * coeffs[j][k];
        }
      }

      for (int j = 0; j < OversamplingFactor; ++j)
      {
        DataType z = static_cast<DataType>(j) / OversamplingFactor - 1 / 2.;

        DataType temp = 0;
        for(int k = OversamplingElements::order; k >= 0; --k)
        {
          temp = temp * z + c[k];
        }
        out[OversamplingFactor * i + j] = temp;
      }
    }
  }
};

}

#endif
