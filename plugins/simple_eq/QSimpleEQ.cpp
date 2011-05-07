/**
 * \file QSimpleEQ.cpp
 */

#include <boost/lexical_cast.hpp>

#include <QtGui/QGridLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QGroupBox>

#include <qevent.h>

#include "QSimpleEQ.h"
#include "simple_eq_effect.h"

QSimpleEQ::QSimpleEQ(SimpleEQEffect* simple_eq, HWND h_parent)
:QWinWidget(h_parent, NULL), simple_eq(simple_eq), h_parent(h_parent)
{
  setAttribute(Qt::WA_DeleteOnClose);

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(create_LF(), 0, 0);
  layout->addWidget(create_LMF(), 0, 1);
  layout->addWidget(create_HMF(), 1, 0);
  layout->addWidget(create_HF(), 1, 1);

  QString css = "QGroupBox { "
"    color: white;"
"    background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgb(116, 116, 118), stop:1 rgb(59, 57, 60));"
"    border: 2px solid rgb(180, 180, 180); "
"    border-radius: 7px;"
"    padding-top: 1.5em;"
"}"
"QSimpleEQ { "
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

  connect(gain_slider_lf, SIGNAL(valueChanged(int)), this, SLOT(update_gain_lf(int)));

  update_gain_lf(simple_eq->getParameter(0));
}

QWidget* QSimpleEQ::create_LF()
{
  QGroupBox* box = new QGroupBox(this);
  box->setTitle("Low frequency");
  gain_slider_lf = new QtSVGDial;
  gain_slider_lf->setSkin("Beryl");
  gain_slider_lf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider_lf->setMinimum(-200);
  gain_slider_lf->setMaximum(200);

  cut_slider_lf = new QtSVGDial;
  cut_slider_lf->setSkin("Beryl");
  cut_slider_lf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  cut_slider_lf->setMinimum(-200);
  cut_slider_lf->setMaximum(200);

  QGridLayout *layout = new QGridLayout(box);
  layout->addWidget(gain_slider_lf, 0, 1, 2, 1);
  layout->addWidget(cut_slider_lf, 1, 2, 2, 1);

  box->setLayout(layout);
  
  return box;
}

QWidget* QSimpleEQ::create_LMF()
{
  QGroupBox* box = new QGroupBox(this);
  box->setTitle("Medium-low frequency");
  Q_slider_lmf = new QtSVGDial;
  Q_slider_lmf->setSkin("Beryl");
  Q_slider_lmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  Q_slider_lmf->setMinimum(1);
  Q_slider_lmf->setMaximum(20);

  gain_slider_lmf = new QtSVGDial;
  gain_slider_lmf->setSkin("Beryl");
  gain_slider_lmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider_lmf->setMinimum(-200);
  gain_slider_lmf->setMaximum(200);

  cut_slider_lmf = new QtSVGDial;
  cut_slider_lmf->setSkin("Beryl");
  cut_slider_lmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  cut_slider_lmf->setMinimum(-200);
  cut_slider_lmf->setMaximum(200);

  QGridLayout *layout = new QGridLayout(box);
  layout->addWidget(Q_slider_lmf, 1, 0, 2, 1);
  layout->addWidget(gain_slider_lmf, 0, 1, 2, 1);
  layout->addWidget(cut_slider_lmf, 1, 2, 2, 1);

  box->setLayout(layout);
  
  return box;
}

QWidget* QSimpleEQ::create_HMF()
{
  QGroupBox* box = new QGroupBox(this);
  box->setTitle("Medium-high frequency");
  Q_slider_hmf = new QtSVGDial;
  Q_slider_hmf->setSkin("Beryl");
  Q_slider_hmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  Q_slider_hmf->setMinimum(1);
  Q_slider_hmf->setMaximum(20);

  gain_slider_hmf = new QtSVGDial;
  gain_slider_hmf->setSkin("Beryl");
  gain_slider_hmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider_hmf->setMinimum(-200);
  gain_slider_hmf->setMaximum(200);

  cut_slider_hmf = new QtSVGDial;
  cut_slider_hmf->setSkin("Beryl");
  cut_slider_hmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  cut_slider_hmf->setMinimum(-200);
  cut_slider_hmf->setMaximum(200);

  QGridLayout *layout = new QGridLayout(box);
  layout->addWidget(Q_slider_hmf, 1, 0, 2, 1);
  layout->addWidget(gain_slider_hmf, 0, 1, 2, 1);
  layout->addWidget(cut_slider_hmf, 1, 2, 2, 1);

  box->setLayout(layout);
  
  return box;
}

QWidget* QSimpleEQ::create_HF()
{
  QGroupBox* box = new QGroupBox(this);
  box->setTitle("High frequency");
  gain_slider_hf = new QtSVGDial;
  gain_slider_hf->setSkin("Beryl");
  gain_slider_hf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider_hf->setMinimum(-200);
  gain_slider_hf->setMaximum(200);

  cut_slider_hf = new QtSVGDial;
  cut_slider_hf->setSkin("Beryl");
  cut_slider_hf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  cut_slider_hf->setMinimum(-200);
  cut_slider_hf->setMaximum(200);

  QGridLayout *layout = new QGridLayout(box);
  layout->addWidget(gain_slider_hf, 0, 1, 2, 1);
  layout->addWidget(cut_slider_hf, 1, 2, 2, 1);

  box->setLayout(layout);
  
  return box;
}

void QSimpleEQ::update_gain_lf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  gain_slider_lf->setValue(intValue);  
}

void QSimpleEQ::update_gain_lmf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  gain_slider_lmf->setValue(intValue);  
}

void QSimpleEQ::update_gain_hmf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  gain_slider_hmf->setValue(intValue);  
}

void QSimpleEQ::update_gain_hf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  gain_slider_hf->setValue(intValue);  
}

void QSimpleEQ::update_cut_lf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  cut_slider_lf->setValue(intValue);  
}

void QSimpleEQ::update_cut_lmf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  cut_slider_lmf->setValue(intValue);  
}

void QSimpleEQ::update_cut_hmf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  cut_slider_hmf->setValue(intValue);  
}

void QSimpleEQ::update_cut_hf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  cut_slider_hf->setValue(intValue);  
}

void QSimpleEQ::update_Q_lmf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  Q_slider_lmf->setValue(intValue);  
}

void QSimpleEQ::update_Q_hmf(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 100);
  Q_slider_hmf->setValue(intValue);  
}

void QSimpleEQ::update_setshelf_lf(bool shelf)
{
}

void QSimpleEQ::update_setshelf_hf(bool shelf)
{
}

void QSimpleEQ::update_gain_lf(int value)
{
  simple_eq->setParameter(0, std::pow(10, value / 100.));
}

void QSimpleEQ::update_gain_lmf(int value)
{
  simple_eq->setParameter(1, std::pow(10, value / 100.));
}

void QSimpleEQ::update_gain_hmf(int value)
{
  simple_eq->setParameter(2, std::pow(10, value / 100.));
}

void QSimpleEQ::update_gain_hf(int value)
{
  simple_eq->setParameter(3, std::pow(10, value / 100.));
}

void QSimpleEQ::update_cut_lf(int value)
{
  simple_eq->setParameter(4, std::pow(10, value / 100.));
}

void QSimpleEQ::update_cut_lmf(int value)
{
  simple_eq->setParameter(5, std::pow(10, value / 100.));
}

void QSimpleEQ::update_cut_hmf(int value)
{
  simple_eq->setParameter(6, std::pow(10, value / 100.));
}

void QSimpleEQ::update_cut_hf(int value)
{
  simple_eq->setParameter(7, std::pow(10, value / 100.));
}

void QSimpleEQ::update_Q_lmf(int value)
{
  simple_eq->setParameter(8, std::pow(10, value / 100.));
}

void QSimpleEQ::update_Q_hmf(int value)
{
  simple_eq->setParameter(9, std::pow(10, value / 100.));
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
