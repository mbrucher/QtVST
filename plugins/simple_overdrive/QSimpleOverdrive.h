/**
 * \file QSimpleOverdrive.h
 */

#include <qwinwidget.h>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include "../../gui/QtScrollDial"

class SimpleOverdriveEffect;

class QSimpleOverdrive : public QWinWidget
{
  Q_OBJECT
  
  HWND h_parent;
  SimpleOverdriveEffect* simple_overdrive;
  QtScrollDial* gain_slider;
  QComboBox* type_combo;
  float sample_rate;

  void clientResize(int width, int height);
  
public slots:
  void update_gain(int value);
  void update_gain(float value);
  void update_oversampling(int value);
  
public:
  QSimpleOverdrive(SimpleOverdriveEffect* simple_overdrive, HWND h_parent = NULL);
};
