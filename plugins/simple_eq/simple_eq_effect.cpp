/**
 * \file simple_eq_effect.cpp
 */

#include <QObject>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include "simple_eq_effect.h"
#include "simple_eq_gui.h"

#include "..\..\blocks\second_order_filter.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new SimpleEQEffect (audioMaster);
}

SimpleEQEffect::SimpleEQEffect (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 1), gain_lf(1), gain_lmf(1), gain_hmf(1), gain_hf(1), cut_lf(100), cut_lmf(1000), cut_hmf(5000), cut_hf(10000), Q_lmf(1), Q_hmf(1), chunk(NULL), size(0)	// 1 program, 1 parameter only
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

  create_effects();  
  connect(this, SIGNAL(update_gain_lf(float)), simple_eq, SIGNAL(update_gain_lf(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_gain_lmf(float)), simple_eq, SIGNAL(update_gain_lmf(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_gain_hmf(float)), simple_eq, SIGNAL(update_gain_hmf(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_gain_hf(float)), simple_eq, SIGNAL(update_gain_hf(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_cut_lf(float)), simple_eq, SIGNAL(update_cut_lf(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_cut_lmf(float)), simple_eq, SIGNAL(update_cut_lmf(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_cut_hmf(float)), simple_eq, SIGNAL(update_cut_hmf(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_cut_hf(float)), simple_eq, SIGNAL(update_cut_hf(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_Q_lmf(float)), simple_eq, SIGNAL(update_Q_lmf(float)), Qt::QueuedConnection);
  connect(this, SIGNAL(update_Q_hmf(float)), simple_eq, SIGNAL(update_Q_hmf(float)), Qt::QueuedConnection);
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
  chunk = reinterpret_cast<char*>(malloc(10 * sizeof(float)));
  *data = reinterpret_cast<void*>(chunk);
  
  return (10 * sizeof(float));
}

VstInt32 SimpleEQEffect::setChunk (void *data, VstInt32 byteSize, bool isPreset)
{
  if(byteSize < sizeof(float) + sizeof(int))
  {
    return 0;
  }
  setParameter(0, *reinterpret_cast<float*>(data));
  create_effects();

  return 10 * sizeof(float);
}

void SimpleEQEffect::create_effects ()
{
  boost::lock_guard<boost::mutex> lock(mutex);
  size = 0;
  low_shelving_filter.reset(create_low_shelving());
  low_peak_filter.reset(create_low_peak());
  high_peak_filter.reset(create_high_peak());
  high_shelving_filter.reset(create_high_shelving());
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
  create_effects();
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
//      gain_filter->set_gain(value);
//      gain = value;
      emit update_gain_lf(value);
      break;
    }
  }
}

float SimpleEQEffect::getParameter (VstInt32 index)
{
  switch(index)
  {
    case 0:
	  return 0;
//      return gain;
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
      float2string (gain_lf, text, kVstMaxParamStrLen);
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
  low_shelving_filter->process(inputs[0], temp_array.get(), sampleFrames);
  low_peak_filter->process(temp_array.get(), temp_array2.get(), sampleFrames);
  high_peak_filter->process(temp_array2.get(), temp_array.get(), sampleFrames);
  high_shelving_filter->process(temp_array.get(), outputs[0], sampleFrames);
}

void SimpleEQEffect::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
  boost::lock_guard<boost::mutex> lock(mutex);

  resize(sampleFrames);
  low_shelving_filter->process(inputs[0], temp_array.get(), sampleFrames);
  low_peak_filter->process(temp_array.get(), temp_array2.get(), sampleFrames);
  high_peak_filter->process(temp_array2.get(), temp_array.get(), sampleFrames);
  high_shelving_filter->process(temp_array.get(), outputs[0], sampleFrames);
}

DSP::MonoFilter<double>* SimpleEQEffect::create_low_shelving()
{
  DSP::SecondOrderFilter<DSP::LowShelvingCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::LowShelvingCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(cut_lf);
  filter->set_gain(gain_lf);

  return filter;
}

DSP::MonoFilter<double>* SimpleEQEffect::create_low_peak()
{
  DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(cut_lmf);
  filter->set_gain(gain_lmf);
  filter->set_Q(Q_lmf);

  return filter;
}

DSP::MonoFilter<double>* SimpleEQEffect::create_high_peak()
{
  DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(cut_hmf);
  filter->set_gain(gain_hmf);
  filter->set_Q(Q_hmf);

  return filter;
}

DSP::MonoFilter<double>* SimpleEQEffect::create_high_shelving()
{
  DSP::SecondOrderFilter<DSP::HighShelvingCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::HighShelvingCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(gain_hf);
  filter->set_gain(gain_hf);

  return filter;
}

void SimpleEQEffect::resize(int new_size)
{
  if(size < new_size)
  {
    temp_array.reset(new double[new_size]);
    temp_array2.reset(new double[new_size]);
    size = new_size;
  }
}

