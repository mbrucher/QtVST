/**
 * \file simple_eq_gui.h
 */

#ifndef __SIMPLEEQGUI__
#define __SIMPLEEQGUI__

#include <qwinwidget.h>

#include <aeffeditor.h>

class SimpleEQEffect;

class GUISimpleEQ : public QObject, public AEffEditor
{
  Q_OBJECT
  
  QWinWidget* widget;
  SimpleEQEffect* effect;

public:
  GUISimpleEQ(SimpleEQEffect* effect)
    :widget(NULL), effect(effect)
  {
  }

  ~GUISimpleEQ()
  {
  }

  virtual bool open(void* ptr);
  virtual bool getRect (ERect** rect);
  virtual void close();

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

  void update_setshelf_lf(bool shelf);
  void update_setshelf_hf(bool shelf);

protected:
  ERect rectangle;
};

void clientResize(HWND h_parent, int width, int height);

#endif
