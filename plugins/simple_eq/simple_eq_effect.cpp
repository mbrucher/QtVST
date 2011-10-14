/**
 * \file simple_eq_effect.cpp
 */

#include <QObject>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

#include "simple_eq_effect.h"
#include "simple_eq_gui.h"

#include "..\..\blocks\second_order_filter.h"

template<int minimum, int range>
float convert_VST_from_ui(int gain)
{
  return (gain + static_cast<float>(minimum)) / range;
}

template<int minimum, int range>
int convert_VST_to_ui(float value)
{
  return value * range - minimum;
}

float convert_db_from_ui(int value)
{
  return std::pow(10, value / 100.);
}

int convert_db_to_ui(float gain)
{
  return (std::log(gain) / std::log(10.f) * 100);
}

static float convert_from_gain(float gain)
{
  float value = convert_db_to_ui(gain);
  return convert_VST_from_ui<200, 400>(value);
}

static float convert_to_gain(float value)
{
  value = convert_VST_to_ui<200, 400>(value);
  return convert_db_from_ui(value);
}

static float convert_from_cut(float cut)
{
  float value = convert_db_to_ui(cut);
  return convert_VST_from_ui<0, 300>(value);
}

static float convert_to_cut(float cut)
{
  cut = convert_VST_to_ui<0, 300>(cut);
  return convert_db_from_ui(cut);
}

static float convert_from_Q(float Q)
{
  float value = convert_db_to_ui(Q);
  return convert_VST_from_ui<1, 100>(value);
}

static float convert_to_Q(float Q)
{
  Q = convert_VST_to_ui<1, 100>(Q);
  return convert_db_from_ui(Q);
}

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new SimpleEQEffect (audioMaster);
}

SimpleEQEffect::SimpleEQEffect (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 10), gain_lf(1), gain_lmf(1), gain_hmf(1), gain_hf(1), cut_lf(100), cut_lmf(1000), cut_hmf(5000), cut_hf(10000), Q_lmf(1), Q_hmf(1), chunk(NULL), size(0)	// 1 program, 1 parameter only
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
  update_effects();
}

void SimpleEQEffect::update_effects ()
{
  low_shelving_filter->set_sampling_frequency(sample_rate);
  low_shelving_filter->set_cut_frequency(cut_lf);
  low_shelving_filter->set_gain(gain_lf);

  low_peak_filter->set_sampling_frequency(sample_rate);
  low_peak_filter->set_cut_frequency(cut_lmf);
  low_peak_filter->set_gain(gain_lmf);
  low_peak_filter->set_Q(Q_lmf);

  high_peak_filter->set_sampling_frequency(sample_rate);
  high_peak_filter->set_cut_frequency(cut_hmf);
  high_peak_filter->set_gain(gain_hmf);
  high_peak_filter->set_Q(Q_hmf);

  high_shelving_filter->set_sampling_frequency(sample_rate);
  high_shelving_filter->set_cut_frequency(gain_hf);
  high_shelving_filter->set_gain(gain_hf);
}

float SimpleEQEffect::getSampleRate ()
{
  return sample_rate;
}
void SimpleEQEffect::setParameter (VstInt32 index, float value)
{
  switch(index)
  {
    case 0:
	  gain_lf = convert_to_gain(value);
      emit update_gain_lf(value);
      break;
    case 1:
	  gain_lmf = convert_to_gain(value);
      emit update_gain_lmf(value);
      break;
    case 2:
	  gain_hmf = convert_to_gain(value);
      emit update_gain_hmf(value);
      break;
    case 3:
	  gain_hf = convert_to_gain(value);
      emit update_gain_hf(value);
      break;
    case 4:
	  cut_lf = convert_to_cut(value);
      emit update_cut_lf(value);
      break;
    case 5:
	  cut_lmf = convert_to_cut(value);
      emit update_cut_lmf(value);
      break;
    case 6:
	  cut_hmf = convert_to_cut(value);
      emit update_Q_hmf(value);
      break;
    case 7:
	  cut_hf = convert_to_cut(value);
      emit update_cut_hf(value);
      break;
    case 8:
	  Q_lmf = convert_to_Q(value);
      emit update_Q_lmf(value);
      break;
    case 9:
	  Q_hmf = convert_to_Q(value);
      emit update_Q_hmf(value);
      break;
  }
  update_effects();
}

float SimpleEQEffect::getParameter (VstInt32 index)
{
  switch(index)
  {
    case 0:
	  return convert_from_gain(gain_lf);
    case 1:
	  return convert_from_gain(gain_lmf);
    case 2:
	  return convert_from_gain(gain_hmf);
    case 3:
	  return convert_from_gain(gain_hf);
    case 4:
	  return convert_from_cut(cut_lf);
    case 5:
	  return convert_from_cut(cut_lmf);
    case 6:
	  return convert_from_cut(cut_hmf);
    case 7:
	  return convert_from_cut(cut_hf);
    case 8:
	  return convert_from_Q(Q_lmf);
    case 9:
	  return convert_from_Q(Q_hmf);
  }
}

void SimpleEQEffect::getParameterName (VstInt32 index, char* label)
{
  switch(index)
  {
    case 0:
	    vst_strncpy (label, "Gain LF", kVstMaxParamStrLen);
      break;
    case 1:
	    vst_strncpy (label, "Gain LMF", kVstMaxParamStrLen);
      break;
    case 2:
	    vst_strncpy (label, "Gain HMF", kVstMaxParamStrLen);
      break;
    case 3:
	    vst_strncpy (label, "Gain HF", kVstMaxParamStrLen);
      break;
    case 4:
	    vst_strncpy (label, "Cut LF", kVstMaxParamStrLen);
      break;
    case 5:
	    vst_strncpy (label, "Cut LMF", kVstMaxParamStrLen);
      break;
    case 6:
	    vst_strncpy (label, "Cut HMF", kVstMaxParamStrLen);
      break;
    case 7:
	    vst_strncpy (label, "Cut HF", kVstMaxParamStrLen);
      break;
    case 8:
	    vst_strncpy (label, "Q LMF", kVstMaxParamStrLen);
      break;
    case 9:
	    vst_strncpy (label, "Q HMF", kVstMaxParamStrLen);
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
    case 1:
      float2string (gain_lmf, text, kVstMaxParamStrLen);
      break;
    case 2:
      float2string (gain_hmf, text, kVstMaxParamStrLen);
      break;
    case 3:
      float2string (gain_hf, text, kVstMaxParamStrLen);
      break;
    case 4:
      float2string (cut_lf, text, kVstMaxParamStrLen);
      break;
    case 5:
      float2string (cut_lmf, text, kVstMaxParamStrLen);
      break;
    case 6:
      float2string (cut_hmf, text, kVstMaxParamStrLen);
      break;
    case 7:
      float2string (cut_hf, text, kVstMaxParamStrLen);
      break;
    case 8:
      float2string (Q_lmf, text, kVstMaxParamStrLen);
      break;
    case 9:
      float2string (Q_hmf, text, kVstMaxParamStrLen);
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
    case 1:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
    case 2:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
    case 3:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
    case 4:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
    case 5:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
    case 6:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
    case 7:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
    case 8:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
    case 9:
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

DSP::SecondOrderFilter<DSP::LowShelvingCoefficients<double> >* SimpleEQEffect::create_low_shelving()
{
  DSP::SecondOrderFilter<DSP::LowShelvingCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::LowShelvingCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(cut_lf);
  filter->set_gain(gain_lf);

  return filter;
}

DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* SimpleEQEffect::create_low_peak()
{
  DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(cut_lmf);
  filter->set_gain(gain_lmf);
  filter->set_Q(Q_lmf);

  return filter;
}

DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* SimpleEQEffect::create_high_peak()
{
  DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* filter = new DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >;
  filter->set_sampling_frequency(sample_rate);
  filter->set_cut_frequency(cut_hmf);
  filter->set_gain(gain_hmf);
  filter->set_Q(Q_hmf);

  return filter;
}

DSP::SecondOrderFilter<DSP::HighShelvingCoefficients<double> >* SimpleEQEffect::create_high_shelving()
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

