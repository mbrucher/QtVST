/**
 * \file newton_raphson_optimizer
 */

#ifndef NEWTONRAPHSONOPTMIZER
#define NEWTONRAPHSONOPTMIZER

#include <iostream>
#include <limits>

namespace DSP
{

template<class Function>
class NewtonRaphsonOptimizer
{
  Function& function;

  typedef typename Function::DataType DataType;

  DataType x0, y0;

  DataType optimize(DataType x1)
  {
    DataType y1 = y0;

     for(int i = 0; i < 10000; ++i)
     {
       DataType prime = function.prime(x0, x1, y0, y1);
       if(std::abs(prime) < std::numeric_limits<DataType>::epsilon() )
       {
         return y1;
       }
      DataType yk = y1 - function(x0, x1, y0, y1) / function.prime(x0, x1, y0, y1);
       if( std::abs(yk - y1) < 0.00001 )
       {
         return yk;
       }
       y1 = yk;
     }
    return y1;
  }
  
public:
  NewtonRaphsonOptimizer(Function& function)
  :function(function), x0(0), y0(0)
  {
  }

  void process(const DataType* in, DataType* out, long size)
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
