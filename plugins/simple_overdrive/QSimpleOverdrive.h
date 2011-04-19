/**
 * \file QSimpleOverdrive.h
 */

#include <qwinwidget.h>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include <QtGui/QSlider>

class AudioEffectX;

class QSimpleOverdrive : public QWinWidget
{
  Q_OBJECT
  
  HWND h_parent;
  AudioEffectX *simple_overdrive;
  QLabel *gain_label;
  QSlider *gain_slider;
  QLabel *type_label;
  QComboBox *type_combo;
  float sample_rate;

  void clientResize(int width, int height);
  
public slots:
  void update_gain(int value);
  void update_gain(float value);
  void update_oversampling(int value);
  
public:
  QSimpleOverdrive(AudioEffectX *simple_overdrive, HWND h_parent = NULL);
};
