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
  DataType dt;
  DataType R;
  DataType C;

public:
  SimpleOverdrive(DataType dt, DataType R, DataType C)
  :dt(dt), R(R), C(C)
  {
  }

};

}

#endif
