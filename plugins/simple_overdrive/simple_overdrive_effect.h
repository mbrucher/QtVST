/**
 * \file variable_filter_main.h
 */

#ifndef __again__
#define __again__

#include <QObject>

#include <boost/scoped_ptr.hpp>

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "..\..\blocks\filter.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster);

//-------------------------------------------------------------------------------------------------------
class SimpleOverdriveEffect : public QObject, public AudioEffectX
{
Q_OBJECT
public:
  SimpleOverdriveEffect (audioMasterCallback audioMaster);
  ~SimpleOverdriveEffect ();

  // Processing
  virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);

  // Program
  virtual void setProgramName (char* name);
  virtual void getProgramName (char* name);

  virtual void   setSampleRate (float sampleRate);
  virtual float   getSampleRate ();
  // Parameters
  virtual void setParameter (VstInt32 index, float value);
  virtual float getParameter (VstInt32 index);
  virtual void getParameterLabel (VstInt32 index, char* label);
  virtual void getParameterDisplay (VstInt32 index, char* text);
  virtual void getParameterName (VstInt32 index, char* text);

  virtual bool getEffectName (char* name);
  virtual bool getVendorString (char* text);
  virtual bool getProductString (char* text);
  virtual VstInt32 getVendorVersion ();

protected:
  boost::scoped_ptr<DSP::MonoFilter<double> > filter;
  char programName[kVstMaxProgNameLen + 1];
  float sample_rate;
  
signals:
  void update_gain(float value);
};

#endif
