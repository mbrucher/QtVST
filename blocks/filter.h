/**
 * \file filter.h
 */

#ifndef DSP_FILTER
#define DSP_FILTER

#include "config.h"

namespace DSP
{

#define DSP_MONOFILTER_DECLARE()\
void process(const float* RESTRICT in, DataType* RESTRICT out, long size)\
{\
  process<float>(in, out, size);\
}\
void process(const double* RESTRICT in, DataType* RESTRICT out, long size)\
{\
  process<double>(in, out, size);\
}\

template<class Data_Type>
class MonoFilter
{
public:
  typedef Data_Type DataType;

  virtual ~MonoFilter();

  virtual void process(const float* RESTRICT in, DataType* RESTRICT out, long size) = 0;
  virtual void process(const double* RESTRICT in, DataType* RESTRICT out, long size) = 0;
};

}

#endif

