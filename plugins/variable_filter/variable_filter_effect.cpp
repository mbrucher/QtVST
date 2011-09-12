/**
 * \file variable_filter_effect.cpp
 */
#include <iostream>
#include <QObject>

#include "variable_filter_effect.h"
#include "variable_filter_gui.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new VariableFilterEffect (audioMaster);
}

VariableFilterEffect::VariableFilterEffect (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, 3)	// 1 program, 1 parameter only
{
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('VaFi');	// identify
	canProcessReplacing ();	// supports replacing output

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name

  GUIVariableFilter* VariableFilterEffect = new GUIVariableFilter(this);

	setEditor(VariableFilterEffect);
  connect(this, SIGNAL(update_frequency(float)), VariableFilterEffect, SIGNAL(update_frequency(float)));
  connect(this, SIGNAL(update_q(float)), VariableFilterEffect, SIGNAL(update_q(float)));
}

VariableFilterEffect::~VariableFilterEffect ()
{
	// nothing to do here
}

void VariableFilterEffect::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

void VariableFilterEffect::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

void VariableFilterEffect::setSampleRate (float sample_rate)
{
  this->sample_rate = sample_rate;
  filter[0].set_sampling_frequency(sample_rate);
  filter[1].set_sampling_frequency(sample_rate);
}

float VariableFilterEffect::getSampleRate ()
{
  return sample_rate;;
}
void VariableFilterEffect::setParameter (VstInt32 index, float value)
{
  switch(index)
  {
    case 0:
    {
      filter[0].set_cutoff_frequency(value * sample_rate / 2);
      filter[1].set_cutoff_frequency(value * sample_rate / 2);
      emit update_frequency(value);
      float numerical_frequency = std::sin(M_PI * value / 2);
      if(filter[0].get_attenuation() > 1 - numerical_frequency)
      {
        setParameter(1, 1 - numerical_frequency);
      }
      break;
    }
    case 1:
      filter[0].set_attenuation(value);
      filter[1].set_attenuation(value);
      emit update_q(value);
      break;
    case 2:
      filter[0].select(static_cast<int>(value));
      filter[1].select(static_cast<int>(value));
      break;
  }
}

float VariableFilterEffect::getParameter (VstInt32 index)
{
  switch(index)
  {
    case 0:
      return filter[0].get_cutoff_frequency() * 2 / sample_rate;
    case 1:
      return filter[0].get_attenuation();
    case 2:
      return filter[0].get_selected();
  }
}

void VariableFilterEffect::getParameterName (VstInt32 index, char* label)
{
  switch(index)
  {
    case 0:
	    vst_strncpy (label, "Cutoff", kVstMaxParamStrLen);
      break;
    case 1:
	    vst_strncpy (label, "Q", kVstMaxParamStrLen);
      break;
    case 2:
	    vst_strncpy (label, "Type", kVstMaxParamStrLen);
      break;
  }
}

void VariableFilterEffect::getParameterDisplay (VstInt32 index, char* text)
{
  switch(index)
  {
    case 0:
      float2string (filter[0].get_cutoff_frequency(), text, kVstMaxParamStrLen);
      break;
    case 1:
      float2string (filter[0].get_attenuation(), text, kVstMaxParamStrLen);
      break;
    case 2:
      float2string (filter[0].get_selected(), text, kVstMaxParamStrLen);
      break;
  }
}

void VariableFilterEffect::getParameterLabel (VstInt32 index, char* label)
{
  switch(index)
  {
    case 0:
	    vst_strncpy (label, "Hz", kVstMaxParamStrLen);
      break;
    case 1:
    case 2:
	    vst_strncpy (label, "", kVstMaxParamStrLen);
      break;
  }
}

bool VariableFilterEffect::getEffectName (char* name)
{
	vst_strncpy (name, "VariableFilter", kVstMaxEffectNameLen);
	return true;
}

bool VariableFilterEffect::getProductString (char* text)
{
	vst_strncpy (text, "VariableFilter", kVstMaxProductStrLen);
	return true;
}

bool VariableFilterEffect::getVendorString (char* text)
{
	vst_strncpy (text, "Matthieu Brucher", kVstMaxVendorStrLen);
	return true;
}

VstInt32 VariableFilterEffect::getVendorVersion ()
{ 
	return 1000; 
}

void VariableFilterEffect::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
  filter[0].process(inputs[0], outputs[0], sampleFrames);
  filter[1].process(inputs[1], outputs[1], sampleFrames);
}
