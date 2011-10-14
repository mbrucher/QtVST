/**
 * \file simple_eq_effect.h
 */

#ifndef __simpleeq__
#define __simpleeq__

#include <QObject>

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/thread/mutex.hpp>

#include <audioeffectx.h>
#include "..\..\blocks\second_order_filter.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster);

float convert_db_from_ui(int value);
int convert_db_to_ui(float gain);

//-------------------------------------------------------------------------------------------------------
class SimpleEQEffect : public QObject, public AudioEffectX
{
Q_OBJECT
public:
  SimpleEQEffect (audioMasterCallback audioMaster);
  ~SimpleEQEffect ();

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

protected:
  static const int max_frequency = 22000;
  DSP::SecondOrderFilter<DSP::LowShelvingCoefficients<double> >* create_low_shelving();
  DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* create_low_peak();
  DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> >* create_high_peak();
  DSP::SecondOrderFilter<DSP::HighShelvingCoefficients<double> >* create_high_shelving();

  boost::scoped_ptr<DSP::SecondOrderFilter<DSP::LowShelvingCoefficients<double> > > low_shelving_filter;
  boost::scoped_ptr<DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> > > low_peak_filter;
  boost::scoped_ptr<DSP::SecondOrderFilter<DSP::BandPassPeakCoefficients<double> > > high_peak_filter;
  boost::scoped_ptr<DSP::SecondOrderFilter<DSP::HighShelvingCoefficients<double> > > high_shelving_filter;
  char programName[kVstMaxProgNameLen + 1];
  float sample_rate;
  float gain_lf;
  float gain_lmf;
  float gain_hmf;
  float gain_hf;

  float cut_lf;
  float cut_lmf;
  float cut_hmf;
  float cut_hf;

  float Q_lmf;
  float Q_hmf;

  char* chunk;

  boost::mutex mutex;

  int size;
  boost::scoped_array<double> temp_array;
  boost::scoped_array<double> temp_array2;
  
  void resize(int new_size);
  void create_effects ();
  void update_effects ();

signals:
  void update_gain_lf(float value);
  void update_gain_lmf(float value);
  void update_gain_hmf(float value);
  void update_gain_hf(float value);

  void update_cut_lf(float value);
  void update_cut_lmf(float value);
  void update_cut_hmf(float value);
  void update_cut_hf(float value);

  void update_Q_lmf(float value);
  void update_Q_hmf(float value);
};

#endif
