/**
 * \file simple_overdrive.h
 */

#ifndef DSP_SIMPLEOVERDRIVE
#define DSP_SIMPLEOVERDRIVE

#include "diode.h"

namespace DSP
{

template<class DataType>
class SimpleOverdrive
{
  DataType dt;
  DataType R;
  DataType C;

  DataType A;
  DataType B;
  
  Diode<DataType> diode;
public:
  SimpleOverdrive(DataType dt, DataType R, DataType C, DataType is, DataType vt)
  :dt(dt), R(R), C(C), diode(is, vt)
  {
    A = dt / (2 * C) + R;
    B = dt / (2 * C) - R;
  }

  DataType operator()(DataType x0, DataType x1, DataType y0, DataType y1)
  {
    //return self.f(y1) + 1/self.A * (y1 + (x0 - x1 + self.B * self.f(y0) - y0));
  }
/*
  def gradient(self, x0, x1, y0, y1):
    return self.fprime(y1) + 1/self.A*/
    
};

}

#endif
