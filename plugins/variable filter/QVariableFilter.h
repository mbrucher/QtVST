/**
 * \file QVariableFilter.h
 */

#include <qwinwidget.h>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QSlider>

class AudioEffectX;

class QVariableFilter : public QWinWidget
{
  Q_OBJECT
  
  HWND h_parent;
  AudioEffectX *variable_filter;
  QLabel *frequency_label;
  QSlider *frequency_slider;
  QLabel *q_label;
  QSlider *q_slider;
  QComboBox *type_combo;
  float sample_rate;

  void clientResize(int width, int height);
  
public slots:
  void update_frequency(int value);
  void update_frequency(float value);
  void update_q(int value);
  void update_q(float value);
  void update_type(int value);
  
public:
  QVariableFilter(AudioEffectX *again, HWND h_parent = NULL);
};
