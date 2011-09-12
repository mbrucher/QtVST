/**
 * \file newton_raphson_optimizer
 */

#ifndef DSP_NEWTONRAPHSONOPTMIZER
#define DSP_NEWTONRAPHSONOPTMIZER

#include <iostream>
#include <limits>

#include "filter.h"

namespace DSP
{

/**
 * A generic Newton Raphson optimization routine
 */
template<class Function>
class NewtonRaphsonOptimizer: public MonoFilter<typename Function::DataType>
{
  static const int max_iterations = 10000;

  typedef MonoFilter<typename Function::DataType> Parent;
  typedef typename Parent::DataType DataType;
  Function function;

  DataType x0, y0;
  DataType precision;

  DataType optimize(DataType x1)
  {
    DataType y1 = y0;

    for(int i = 0; i < max_iterations; ++i)
    {
      std::pair<DataType, DataType> all = function.all(x0, x1, y0, y1);
      if(std::abs(all.second) < std::numeric_limits<DataType>::epsilon() )
      {
        return y1;
      }
      DataType yk = y1 - all.first / all.second;
      if( std::abs(yk - y1) < precision )
      {
        return yk;
      }
      y1 = yk;
    }
    return y1;
  }

public:
  NewtonRaphsonOptimizer(const Function& function)
  :function(function), x0(0), y0(0), precision(0.00001)
  {
  }

  DSP_MONOFILTER_DECLARE()

  template<class DataTypeIn, class DataTypeOut>
  void process(const DataTypeIn* RESTRICT in, DataTypeOut* RESTRICT out, unsigned long size)
  {
    for(long i = 0; i < size; ++i)
    {
      out[i] = optimize(in[i]);
      if(out[i] < 1.)
      {
        if(!(out[i] > -1))
        {
          out[i] = -1;
        }
      }
      else
      {
        out[i] = 1;
      }
      x0 = in[i];
      y0 = out[i];
    }
  }

};

}

#endif
