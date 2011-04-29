/**
 * \file QSimpleOverdrive.h
 */

#include <qwinwidget.h>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include "../../gui/QtSVGDial"

class SimpleOverdriveEffect;

class QSimpleOverdrive : public QWinWidget
{
  Q_OBJECT
  
  HWND h_parent;
  SimpleOverdriveEffect* simple_overdrive;
  QtSVGDial* gain_slider;
  QComboBox* type_combo;
  float sample_rate;

  void clientResize(int width, int height);
  
public slots:
  void update_gain(int value);
  void update_gain(float value);
  void update_oversampling(int value);
  void update_oversampling_log(int value);

protected:
  void mousePressEvent(QMouseEvent *me);

public:
  QSimpleOverdrive(SimpleOverdriveEffect* simple_overdrive, HWND h_parent = NULL);
};
