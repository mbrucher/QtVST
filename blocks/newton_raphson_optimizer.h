/**
 * \file newton_raphson_optimizer
 */

#ifndef NEWTONRAPHSONOPTMIZER
#define NEWTONRAPHSONOPTMIZER

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
    return x1;
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
	}
  }

};

}

#endif
