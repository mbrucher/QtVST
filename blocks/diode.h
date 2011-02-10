/**
 * \file diode.h
 */

#ifndef DSP_DIODE
#define DSP_DIODE

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
  DataType operator()()
  {
    //return np.sign(x) * is * (np.exp(np.abs(x) / vt) - 1);
  }
  
  DataType prime()
  {
    // return is * np.exp(np.abs(x) / vt) / vt
  }
};

}

#endif
