/**
 * \file guiagain.h
 */

#include <qwinwidget.h>
#include "aeffeditor.h"

class GUIVariableFilter : public QObject, public AEffEditor
{
  Q_OBJECT
  
  QWinWidget* widget;
  AudioEffectX* effect;

public:
  GUIVariableFilter(AudioEffectX* effect)
    :widget(NULL), effect(effect)
  {
  }

  ~GUIVariableFilter()
  {
  }

  virtual bool open(void* ptr);
  virtual bool getRect (ERect** rect);
  virtual void close();

signals:
  void update_frequency(float value);
  void update_q(float value);

protected:
  ERect rectangle;
};

void clientResize(HWND h_parent, int width, int height);
