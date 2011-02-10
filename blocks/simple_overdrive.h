/**
 * \file simple_overdrive.h
 */

#ifndef DSP_SIMPLEOVERDRIVE
#define DSP_SIMPLEOVERDRIVE

namespace DSP
{

template<class DataType>
class SimpleOverdrive
{
  DataType R;
  DataType C;

public:
  SimpleOverdrive(DataType R, DataType C)
  :R(R), C(C)
  {
  }

};

}

#endif
