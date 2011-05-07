/**
 * \file simple_eq_gui.cpp
 */

#include <windows.h>

#include <qwinwidget.h>

#include "simple_eq_gui.h"
#include "QSimpleEQ.h"

bool GUISimpleEQ::open(void* ptr)
{
  AEffEditor::open (ptr);
  widget = new QSimpleEQ(effect, static_cast<HWND>(ptr));
  widget->move( 0, 0 );
  widget->adjustSize();
  rectangle.top = 0;
  rectangle.left = 0;
  rectangle.bottom = widget->height();
  rectangle.right = widget->width();
  widget->setMinimumSize(widget->size());
  widget->show();
  clientResize(static_cast<HWND>(ptr), widget->width(), widget->height());

  connect(this, SIGNAL(update_gain_lf(float)), widget, SLOT(update_gain_lf(float)));
  connect(this, SIGNAL(update_gain_lmf(float)), widget, SLOT(update_gain_lmf(float)));
  connect(this, SIGNAL(update_gain_hmf(float)), widget, SLOT(update_gain_hmf(float)));
  connect(this, SIGNAL(update_gain_hf(float)), widget, SLOT(update_gain_hf(float)));

  connect(this, SIGNAL(update_cut_lf(float)), widget, SLOT(update_cut_lf(float)));
  connect(this, SIGNAL(update_cut_lmf(float)), widget, SLOT(update_cut_lmf(float)));
  connect(this, SIGNAL(update_cut_hmf(float)), widget, SLOT(update_cut_hmf(float)));
  connect(this, SIGNAL(update_cut_hf(float)), widget, SLOT(update_cut_hf(float)));

  connect(this, SIGNAL(update_setshelf_hf(bool)), widget, SLOT(update_setshelf_hf(bool)));
  connect(this, SIGNAL(update_setshelf_lf(bool)), widget, SLOT(update_setshelf_lf(bool)));
  connect(this, SIGNAL(update_Q_lmf(float)), widget, SLOT(update_Q_lmf(float)));
  connect(this, SIGNAL(update_Q_hmf(float)), widget, SLOT(update_Q_hmf(float)));
  return true;
}

void GUISimpleEQ::close()
{
  disconnect(this, SIGNAL(update_gain_lf(float)), widget, SLOT(update_gain_lf(float)));
  disconnect(this, SIGNAL(update_gain_lmf(float)), widget, SLOT(update_gain_lmf(float)));
  disconnect(this, SIGNAL(update_gain_hmf(float)), widget, SLOT(update_gain_hmf(float)));
  disconnect(this, SIGNAL(update_gain_hf(float)), widget, SLOT(update_gain_hf(float)));

  disconnect(this, SIGNAL(update_cut_lf(float)), widget, SLOT(update_cut_lf(float)));
  disconnect(this, SIGNAL(update_cut_lmf(float)), widget, SLOT(update_cut_lmf(float)));
  disconnect(this, SIGNAL(update_cut_hmf(float)), widget, SLOT(update_cut_hmf(float)));
  disconnect(this, SIGNAL(update_cut_hf(float)), widget, SLOT(update_cut_hf(float)));

  disconnect(this, SIGNAL(update_setshelf_hf(bool)), widget, SLOT(update_setshelf_hf(bool)));
  disconnect(this, SIGNAL(update_setshelf_lf(bool)), widget, SLOT(update_setshelf_lf(bool)));
  disconnect(this, SIGNAL(update_Q_lmf(float)), widget, SLOT(update_Q_lmf(float)));
  disconnect(this, SIGNAL(update_Q_hmf(float)), widget, SLOT(update_Q_hmf(float)));
  delete widget;
}

bool GUISimpleEQ::getRect (ERect** rect)
{
  *rect = &rectangle;
  return true;
}

void clientResize(HWND h_parent, int width, int height)
{
  RECT rcClient, rcWindow;
  POINT ptDiff;
  GetClientRect(h_parent, &rcClient);
  GetWindowRect(h_parent, &rcWindow);
  ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
  ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
  MoveWindow(h_parent, rcWindow.left, rcWindow.top, width + ptDiff.x, height + ptDiff.y, TRUE);
}
