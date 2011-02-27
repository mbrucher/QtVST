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
public:
  NewtonRaphsonOptimizer(Function& function)
  :function(function)
  {
  }

  void process(const typename Function::DataType* in, typename Function::DataType* out, long size)
  {
    for(long i = 0; i < size; ++i)
	{
	  out[i] = in[i];
	}
  }

};

}

#endif
