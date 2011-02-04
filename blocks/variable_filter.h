/**
 * \file variable_filter.hpp
 */

#ifndef VARIABLEFILTER
#define VARIABLEFILTER

#include <iostream>

#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265
#endif

namespace DSP
{

template<class Data_Type>
class VariableFilter
{
public:
  typedef Data_Type DataType;

  VariableFilter()
    :yh(0), yb(0), yl(0), selected(0), cutoff_frequency(0), attenuation(0)
  {
  }

  void process(const DataType* in, DataType* out, unsigned long nb_samples)
  {
    for(unsigned long i = 0; i < nb_samples; ++i)
    {
      yh = in[i] - yl - numerical_attenuation * yb;
      yb = numerical_frequency * yh + yb;
      yl = numerical_frequency * yb + yl;
      if(selected == 0)
      {
        out[i] = yl;
      }
      else if(selected == 1)
      {
        out[i] = yb;
      }
      else
      {
        out[i] = yh;
      }
    }
  } 

  void set_sampling_frequency(DataType sampling_frequency)
  {
    this->sampling_frequency = sampling_frequency;
    compute_factors();
  }

  DataType get_sampling_frequency() const
  {
    return sampling_frequency;
  }

  void set_cutoff_frequency(DataType cutoff_frequency)
  {
    this->cutoff_frequency = cutoff_frequency;
    compute_factors();
  }

  DataType get_cutoff_frequency() const
  {
    return cutoff_frequency;
  }

  void set_attenuation(DataType attenuation)
  {
    this->attenuation = attenuation;
    compute_factors();
  }

  DataType get_attenuation() const
  {
    return attenuation;
  }

  void select(int selected)
  {
    this->selected = selected;
  }

  int get_selected() const
  {
    return selected;
  }

protected:
  void compute_factors()
  {
    numerical_frequency = 2 * std::sin(M_PI * cutoff_frequency / sampling_frequency);
    numerical_attenuation = 2 * attenuation;
  }

private:
  DataType cutoff_frequency;
  DataType attenuation;

  DataType sampling_frequency;
  DataType numerical_frequency;
  DataType numerical_attenuation;

  DataType yh;
  DataType yb;
  DataType yl;
  
  int selected;
};

}
#endif
