/**
 * \file QSimpleOverdrive.h
 */

#ifndef __QSIMPLEEQ__
#define __QSIMPLEEQ__

#include <qwinwidget.h>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QLabel>
#include "../../gui/QtSVGDial"

class SimpleEQEffect;

class QSimpleEQ : public QWinWidget
{
  Q_OBJECT
  
  HWND h_parent;
  SimpleEQEffect* simple_eq;
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
  QSimpleEQ(SimpleEQEffect* simple_eq, HWND h_parent = NULL);
};

#endif