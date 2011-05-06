/**
 * \file simple_eq_effect.cpp
 */

#include <QObject>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include "simple_eq_effect.h"
#include "simple_eq_gui.h"

#include "..\..\blocks\newton_raphson_optimizer.h"
#include "..\..\blocks\simple_overdrive.h"
#include "..\..\blocks\oversampling_filter.h"
#include "..\..\blocks\gain_filter.h"
#include "..\..\blocks\butterworth_filter.h"
#include "..\..\blocks\decimation_filter.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new SimpleEQEffect (audioMaster);
}

SimpleEQEffect::SimpleEQEffect (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 1), gain(1), oversampling(2), chunk(NULL), size(0)	// 1 program, 1 parameter only
{
  setNumInputs (1);		// mono in
  setNumOutputs (1);		// mono out
  setUniqueID ('SiEQ');	// identify
  canProcessReplacing ();	// supports replacing output
  canDoubleReplacing ();	// supports replacing output
  programsAreChunks();
  setInitialDelay(3);

  vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

  GUISimpleEQ* simple_eq = new GUISimpleEQ(this);

  setEditor(simple_eq);
  gain_filter.reset(create_gain_filter());
  connect(this, SIGNAL(update_gain(float)), simple_eq, SIGNAL(update_gain(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_oversampling(int)), simple_eq, SIGNAL(update_oversampling(int)), Qt::QueuedConnection);

  create_effects(oversampling);  
}

void SimpleEQEffect::resume()
{
  if(chunk)
  {
    free(chunk);
	chunk = NULL;
  }
  AudioEffectX::resume();
}

void SimpleEQEffect::suspend()
{
  if(chunk)
  {
    free(chunk);
	chunk = NULL;
  }
  AudioEffectX::suspend();
}
  
VstInt32 SimpleEQEffect::getChunk (void **data, bool isPreset)
{
  chunk = reinterpret_cast<char*>(malloc(sizeof(float) + sizeof(int)));
  *data = reinterpret_cast<void*>(chunk);
  *reinterpret_cast<float*>(chunk) = gain;
  *reinterpret_cast<int*>(chunk + sizeof(float)) = oversampling;
  
  return (sizeof(float) + sizeof(int));
}

VstInt32 SimpleEQEffect::setChunk (void *data, VstInt32 byteSize, bool isPreset)
{
  if(byteSize < sizeof(float) + sizeof(int))
  {
    return 0;
  }
  setParameter(0, *reinterpret_cast<float*>(data));
  create_effects(*reinterpret_cast<int*>(reinterpret_cast<char*>(data) + sizeof(float)));

  return sizeof(float) + sizeof(int);
}

void SimpleEQEffect::create_effects (int oversampling)
{
  boost::lock_guard<boost::mutex> lock(mutex);

  this->oversampling = oversampling;
  size = 0;
  oversampling_filter.reset(create_oversampling_filter());
  overdrive_filter.reset(create_overdrive_filter());
  decimation_low_filter.reset(create_decimation_low_filter());
}

SimpleEQEffect::~SimpleEQEffect ()
{
	// nothing to do here
}

void SimpleEQEffect::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

void SimpleEQEffect::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

void SimpleEQEffect::setSampleRate (float sample_rate)
{
  this->sample_rate = sample_rate;
  create_effects(oversampling);
}

float SimpleEQEffect::getSampleRate ()
{
  return sample_rate;;
}
void SimpleEQEffect::setParameter (VstInt32 index, float value)
{
  switch(index)
  {
    case 0:
    {
      gain_filter->set_gain(value);
      gain = value;
      emit update_gain(value);
      break;
    }
  }
}

float SimpleEQEffect::getParameter (VstInt32 index)
{
  switch(index)
  {
    case 0:
      return gain;
  }
}

void SimpleEQEffect::getParameterName (VstInt32 index, char* label)
{
  switch(index)
  {
    case 0:
	    vst_strncpy (label, "Gain", kVstMaxParamStrLen);
      break;
  }
}

void SimpleEQEffect::getParameterDisplay (VstInt32 index, char* text)
{
  switch(index)
  {
    case 0:
      float2string (gain, text, kVstMaxParamStrLen);
      break;
  }
}

void SimpleEQEffect::getParameterLabel (VstInt32 index, char* label)
{
  switch(index)
  {
    case 0:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
  }
}

bool SimpleEQEffect::getEffectName (char* name)
{
	vst_strncpy (name, "SimpleEQ", kVstMaxEffectNameLen);
	return true;
}

bool SimpleEQEffect::getProductString (char* text)
{
	vst_strncpy (text, "SimpleEQ", kVstMaxProductStrLen);
	return true;
}

bool SimpleEQEffect::getVendorString (char* text)
{
	vst_strncpy (text, "Matthieu Brucher", kVstMaxVendorStrLen);
	return true;
}

VstInt32 SimpleEQEffect::getVendorVersion ()
{ 
	return 1000; 
}

void SimpleEQEffect::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
  boost::lock_guard<boost::mutex> lock(mutex);

  resize(sampleFrames);
  gain_filter->process(inputs[0], gain_array.get(), sampleFrames);
  oversampling_filter->process(gain_array.get(), in_oversampled_array.get(), sampleFrames);
  overdrive_filter->process(in_oversampled_array.get(), out_oversampled_array.get(), sampleFrames * oversampling);
  decimation_low_filter->process(in_oversampled_array.get(), outputs[0], sampleFrames * oversampling);
}

void SimpleEQEffect::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
  boost::lock_guard<boost::mutex> lock(mutex);

  resize(sampleFrames);
  gain_filter->process(inputs[0], gain_array.get(), sampleFrames);
  oversampling_filter->process(gain_array.get(), in_oversampled_array.get(), sampleFrames);
  overdrive_filter->process(in_oversampled_array.get(), out_oversampled_array.get(), sampleFrames * oversampling);
  decimation_low_filter->process(out_oversampled_array.get(), outputs[0], sampleFrames * oversampling);
}

DSP::GainFilter<double>* SimpleEQEffect::create_gain_filter()
{
  DSP::GainFilter<double>* gain_filter = new DSP::GainFilter<double>;
  gain_filter->set_gain(gain);
  return gain_filter;
}

DSP::MonoFilter<double>* SimpleEQEffect::create_oversampling_filter()
{
  DSP::MonoFilter<double>* oversampling_filter;
  switch (oversampling)
  {
    case 2:
      oversampling_filter = new DSP::OversamplingFilter<2, DSP::Oversampling6points5order<double> >;
      break;
    case 4:
      oversampling_filter = new DSP::OversamplingFilter<4, DSP::Oversampling6points5order<double> >;
      break;
    case 8:
      oversampling_filter = new DSP::OversamplingFilter<8, DSP::Oversampling6points5order<double> >;
      break;
    case 16:
      oversampling_filter = new DSP::OversamplingFilter<16, DSP::Oversampling6points5order<double> >;
      break;
    case 32:
      oversampling_filter = new DSP::OversamplingFilter<32, DSP::Oversampling6points5order<double> >;
      break;
  }

  return oversampling_filter;
}

DSP::MonoFilter<double>* SimpleEQEffect::create_overdrive_filter()
{
  DSP::NewtonRaphsonOptimizer<DSP::SimpleOverdrive<double> >* filter = new DSP::NewtonRaphsonOptimizer<DSP::SimpleOverdrive<double> >(DSP::SimpleOverdrive<double>(1./sample_rate / oversampling, 10000, 22e-9, 1e-12, 26e-3));

  return filter;
}

DSP::MonoFilter<double>* SimpleEQEffect::create_decimation_low_filter()
{
  switch(oversampling)
  {
    case 2:
    {
      DSP::DecimationFilter<2, DSP::ButterworthFilter<double, 8> >* decimation_low_filter = new DSP::DecimationFilter<2, DSP::ButterworthFilter<double, 8> >;

      decimation_low_filter->get_filter().set_sampling_frequency(sample_rate * oversampling);
      decimation_low_filter->get_filter().set_cut_frequency(max_frequency);
      return decimation_low_filter;
    }
    case 4:
    {
      DSP::DecimationFilter<4, DSP::ButterworthFilter<double, 8> >* decimation_low_filter = new DSP::DecimationFilter<4, DSP::ButterworthFilter<double, 8> >;

      decimation_low_filter->get_filter().set_sampling_frequency(sample_rate * oversampling);
      decimation_low_filter->get_filter().set_cut_frequency(max_frequency);
      return decimation_low_filter;
    }
    case 8:
    {
      DSP::DecimationFilter<8, DSP::ButterworthFilter<double, 8> >* decimation_low_filter = new DSP::DecimationFilter<8, DSP::ButterworthFilter<double, 8> >;

      decimation_low_filter->get_filter().set_sampling_frequency(sample_rate * oversampling);
      decimation_low_filter->get_filter().set_cut_frequency(max_frequency);
      return decimation_low_filter;
    }
    case 16:
    {
      DSP::DecimationFilter<16, DSP::ButterworthFilter<double, 8> >* decimation_low_filter = new DSP::DecimationFilter<16, DSP::ButterworthFilter<double, 8> >;

      decimation_low_filter->get_filter().set_sampling_frequency(sample_rate * oversampling);
      decimation_low_filter->get_filter().set_cut_frequency(max_frequency);
      return decimation_low_filter;
    }
    case 32:
    {
      DSP::DecimationFilter<32, DSP::ButterworthFilter<double, 8> >* decimation_low_filter = new DSP::DecimationFilter<32, DSP::ButterworthFilter<double, 8> >;

      decimation_low_filter->get_filter().set_sampling_frequency(sample_rate * oversampling);
      decimation_low_filter->get_filter().set_cut_frequency(max_frequency);
      return decimation_low_filter;
    }
  }

  return NULL;
}

void SimpleEQEffect::resize(int new_size)
{
  if(size < new_size)
  {
    gain_array.reset(new double[new_size]);
    in_oversampled_array.reset(new double[new_size * oversampling]);
    out_oversampled_array.reset(new double[new_size * oversampling]);
    size = new_size;
  }
}

void SimpleEQEffect::set_oversampling(int value)
{
  create_effects(value);
  emit update_oversampling(value);
}
