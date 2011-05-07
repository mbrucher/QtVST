/**
 * \file QSimpleEQ.cpp
 */

#include <boost/lexical_cast.hpp>

#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>

#include <qevent.h>

#include "QSimpleEQ.h"
#include "simple_eq_effect.h"

QSimpleEQ::QSimpleEQ(SimpleEQEffect* simple_eq, HWND h_parent)
:QWinWidget(h_parent, NULL), simple_eq(simple_eq), h_parent(h_parent), sample_rate(simple_eq->getSampleRate())
{
  setAttribute(Qt::WA_DeleteOnClose);

  QLabel *gain_label = new QLabel("Gain", this);
  gain_slider = new QtSVGDial(this);
  gain_slider->setSkin("Beryl");
  gain_slider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider->setMinimum(-400);
  gain_slider->setMaximum(400);
  QLabel* type_label = new QLabel("Oversampling", this);
  type_combo = new QComboBox(this);
  type_combo->addItem("2");
  type_combo->addItem("4");
  type_combo->addItem("8");
  type_combo->addItem("16");
  type_combo->addItem("32");

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(gain_label, 0, 0);
  layout->addWidget(gain_slider, 0, 1);
  layout->addWidget(type_label, 1, 0);
  layout->addWidget(type_combo, 1, 1);

  setLayout(layout);
  
  QString css = "QSimpleEQ { "
"    color: white;"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(116, 116, 118), stop:1 rgb(59, 57, 60));"
"    border: 2px solid rgb(180, 180, 180); "
"    border-radius: 7px;"
"    padding-top: 1.5em;"
"}"
"QLabel { "
"    color: white;"
"}";
  setStyleSheet(css);

  connect(gain_slider, SIGNAL(valueChanged(int)), this, SLOT(update_gain(int)));
  connect(type_combo, SIGNAL(activated(int)), this, SLOT(update_oversampling_log(int)));
  
  update_gain_lf(simple_eq->getParameter(0));
}

void QSimpleEQ::update_gain_lf(float value)
{
  simple_eq->setParameter(0, std::pow(10, value / 100.));
}

void QSimpleEQ::update_gain_lmf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  gain_slider->setValue(intValue);  
}

void QSimpleEQ::update_gain_hmf(float value)
{
}

void QSimpleEQ::update_gain_hf(float value)
{
}

void QSimpleEQ::update_cut_lf(float value)
{
}

void QSimpleEQ::update_cut_lmf(float value)
{
}

void QSimpleEQ::update_cut_hmf(float value)
{
}

void QSimpleEQ::update_cut_hf(float value)
{
}

void QSimpleEQ::update_Q_lmf(float value)
{
}

void QSimpleEQ::update_Q_hmf(float value)
{
}

void QSimpleEQ::update_setshelf_lf(bool shelf)
{
}

void QSimpleEQ::update_setshelf_hf(bool shelf)
{
}

void QSimpleEQ::update_gain_lf(int value)
{
}

void QSimpleEQ::update_gain_lmf(int value)
{
}

void QSimpleEQ::update_gain_hmf(int value)
{
}

void QSimpleEQ::update_gain_hf(int value)
{
}

void QSimpleEQ::update_cut_lf(int value)
{
}

void QSimpleEQ::update_cut_lmf(int value)
{
}

void QSimpleEQ::update_cut_hmf(int value)
{
}

void QSimpleEQ::update_cut_hf(int value)
{
}

void QSimpleEQ::update_Q_lmf(int value)
{
}

void QSimpleEQ::update_Q_hmf(int value)
{
}

void QSimpleEQ::mousePressEvent(QMouseEvent *e)
{
  e->accept();
  QMessageBox box;
  box.setWindowTitle("About QtSimpleEQ");
  box.setText("QtSimpleEQ is a VST(tm) Windows plugin made by Matthieu Brucher (<a href=\"http://matt.eifelle.com/\">http://matt.eifelle.com/</a>)");
  box.setIconPixmap(QPixmap(":/VSTLogoAlpha.png"));
  box.exec();
}
