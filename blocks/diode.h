/**
 * \file diode.h
 */

#ifndef DSP_DIODE
#define DSP_DIODE

#include <boost/math/special_functions/sign.hpp>

namespace DSP
{

/**
 * Description of a diode using the Shockley equation
 * @seealso (http://en.wikipedia.org/wiki/Diode)
 */
template<class DataType>
class Diode
{
  DataType is;
  DataType vt;
  
public:
  Diode(DataType is, DataType vt)
  :is(is), vt(vt)
  {
  }

  DataType operator()(DataType x)
  {
    return boost::math::sign(x) * is * (std::exp(std::abs(x) / vt) - 1);
  }
  
  DataType prime(DataType x)
  {
    return is * std::exp(std::abs(x) / vt) / vt;
  }
};

}

#endif
