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
  QtSVGDial* gain_slider_lf;
  QtSVGDial* gain_slider_lmf;
  QtSVGDial* gain_slider_hmf;
  QtSVGDial* gain_slider_hf;

  QtSVGDial* cut_slider_lf;
  QtSVGDial* cut_slider_lmf;
  QtSVGDial* cut_slider_hmf;
  QtSVGDial* cut_slider_hf;

  QtSVGDial* Q_slider_lmf;
  QtSVGDial* Q_slider_hmf;

public slots:
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

  void update_setshelf_lf(bool shelf);
  void update_setshelf_hf(bool shelf);

  void update_gain_lf(int value);
  void update_gain_lmf(int value);
  void update_gain_hmf(int value);
  void update_gain_hf(int value);

  void update_cut_lf(int value);
  void update_cut_lmf(int value);
  void update_cut_hmf(int value);
  void update_cut_hf(int value);

  void update_Q_lmf(int value);
  void update_Q_hmf(int value);

protected:
  void mousePressEvent(QMouseEvent *me);

  QWidget* create_LF();
  QWidget* create_LMF();
  QWidget* create_HMF();
  QWidget* create_HF();

public:
  QSimpleEQ(SimpleEQEffect* simple_eq, HWND h_parent = NULL);
};

#endif