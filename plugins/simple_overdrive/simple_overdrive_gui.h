/**
 * \file simple_overdrive_gui.h
 */

#include <qwinwidget.h>
#include "aeffeditor.h"

class GUISimpleOverdrive : public QObject, public AEffEditor
{
  Q_OBJECT
  
  QWinWidget* widget;
  AudioEffectX* effect;

public:
  GUISimpleOverdrive(AudioEffectX* effect)
    :widget(NULL), effect(effect)
  {
  }

  ~GUISimpleOverdrive()
  {
  }

  virtual bool open(void* ptr);
  virtual bool getRect (ERect** rect);
  virtual void close();

signals:
  void update_gain(float value);

protected:
  ERect rectangle;
};

void clientResize(HWND h_parent, int width, int height);
