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
  void update_gain(float value);
  void update_oversampling(int oversampling);

protected:
  ERect rectangle;
};

void clientResize(HWND h_parent, int width, int height);

#endif
