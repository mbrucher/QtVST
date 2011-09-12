/**
 * \file filter.cpp
 */

#include "filter.h"

namespace DSP
{

template<>
MonoFilter<float>::~MonoFilter()
{
}

template<>
MonoFilter<double>::~MonoFilter()
{
}

}
