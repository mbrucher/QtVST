/**
 * \file variable_filter_main.h
 */

#ifndef __again__
#define __again__

#include <QObject>

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/thread/mutex.hpp>

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "..\..\blocks\gain_filter.h"
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
  virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

  // Program
  virtual void setProgramName (char* name);
  virtual void getProgramName (char* name);

  virtual void setSampleRate (float sampleRate);
  virtual float getSampleRate ();
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

  virtual void resume();
  virtual void suspend();
  
  virtual VstInt32 getChunk (void **data, bool isPreset=false);
  virtual VstInt32 setChunk (void *data, VstInt32 byteSize, bool isPreset=false);

  void set_oversampling(int value);

protected:
  static const int max_frequency = 22000;
  DSP::GainFilter<double>* create_gain_filter();
  DSP::MonoFilter<double>* create_oversampling_filter();
  DSP::MonoFilter<double>* create_overdrive_filter();
  DSP::MonoFilter<double>* create_decimation_low_filter();

  boost::scoped_ptr<DSP::GainFilter<double> > gain_filter;
  boost::scoped_ptr<DSP::MonoFilter<double> > oversampling_filter;
  boost::scoped_ptr<DSP::MonoFilter<double> > overdrive_filter;
  boost::scoped_ptr<DSP::MonoFilter<double> > decimation_low_filter;
  char programName[kVstMaxProgNameLen + 1];
  float sample_rate;
  float gain;
  int oversampling;

  char* chunk;

  boost::mutex mutex;

  int size;
  boost::scoped_array<double> gain_array;
  boost::scoped_array<double> in_oversampled_array;
  boost::scoped_array<double> out_oversampled_array;
  
  void resize(int new_size);
  void create_effects (int oversampling);

signals:
  void update_gain(float value);
  void update_oversampling(int value);
};

#endif
