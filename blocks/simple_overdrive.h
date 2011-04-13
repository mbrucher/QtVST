/**
 * \file simple_overdrive.h
 */

#ifndef DSP_SIMPLEOVERDRIVE
#define DSP_SIMPLEOVERDRIVE

#include "diode.h"

namespace DSP
{

template<class Data_Type>
class SimpleOverdrive
{
public:
  typedef Data_Type DataType;
private:
  DataType dt;
  DataType R;
  DataType C;

  DataType A;
  DataType B;

  Diode<DataType> diode;

  void recompute(DataType dt)
  {
    this->dt = dt;
    A = dt / (2 * C) + R;
    B = dt / (2 * C) - R;
  }
public:
  SimpleOverdrive(DataType dt, DataType R, DataType C, DataType is, DataType vt)
  :dt(dt), R(R), C(C), diode(is, vt)
  {
    recompute(dt);
  }

  DataType operator()(DataType x0, DataType x1, DataType y0, DataType y1)
  {
    return diode(y1) + 1/A * (y1 + (x0 - x1 + B * diode(y0) - y0));
  }

  DataType prime(DataType x0, DataType x1, DataType y0, DataType y1)
  {
    return diode.prime(y1) + 1/A;
  }
};

}

#endif
