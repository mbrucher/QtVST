/**
 * \file simple_overdrive_effect.cpp
 */
#include <iostream>
#include <QObject>

#include "simple_overdrive_effect.h"
#include "simple_overdrive_gui.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new SimpleOverdriveEffect (audioMaster);
}

SimpleOverdriveEffect::SimpleOverdriveEffect (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 1)	// 1 program, 1 parameter only
{
  setNumInputs (1);		// mono in
  setNumOutputs (1);		// mono out
  setUniqueID ('SiOv');	// identify
  canProcessReplacing ();	// supports replacing output

  vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

  GUISimpleOverdrive* simple_overdrive = new GUISimpleOverdrive(this);

  setEditor(simple_overdrive);
  connect(this, SIGNAL(update_gain(float)), simple_overdrive, SIGNAL(update_gain(float)));
  
  gain_filter.reset(new DSP::GainFilter<double>);
}

SimpleOverdriveEffect::~SimpleOverdriveEffect ()
{
	// nothing to do here
}

void SimpleOverdriveEffect::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

void SimpleOverdriveEffect::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

void SimpleOverdriveEffect::setSampleRate (float sample_rate)
{
  this->sample_rate = sample_rate;
}

float SimpleOverdriveEffect::getSampleRate ()
{
  return sample_rate;;
}
void SimpleOverdriveEffect::setParameter (VstInt32 index, float value)
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

float SimpleOverdriveEffect::getParameter (VstInt32 index)
{
  switch(index)
  {
    case 0:
      return gain;
  }
}

void SimpleOverdriveEffect::getParameterName (VstInt32 index, char* label)
{
  switch(index)
  {
    case 0:
	    vst_strncpy (label, "Gain", kVstMaxParamStrLen);
      break;
  }
}

void SimpleOverdriveEffect::getParameterDisplay (VstInt32 index, char* text)
{
  switch(index)
  {
    case 0:
      float2string (gain, text, kVstMaxParamStrLen);
      break;
  }
}

void SimpleOverdriveEffect::getParameterLabel (VstInt32 index, char* label)
{
  switch(index)
  {
    case 0:
	    vst_strncpy (label, "dB", kVstMaxParamStrLen);
      break;
  }
}

bool SimpleOverdriveEffect::getEffectName (char* name)
{
	vst_strncpy (name, "SimpleOverdrive", kVstMaxEffectNameLen);
	return true;
}

bool SimpleOverdriveEffect::getProductString (char* text)
{
	vst_strncpy (text, "SimpleOverdrive", kVstMaxProductStrLen);
	return true;
}

bool SimpleOverdriveEffect::getVendorString (char* text)
{
	vst_strncpy (text, "Matthieu Brucher", kVstMaxVendorStrLen);
	return true;
}

VstInt32 SimpleOverdriveEffect::getVendorVersion ()
{ 
	return 1000; 
}

void SimpleOverdriveEffect::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
  gain_filter->process(inputs[0], outputs[0], sampleFrames);
}
