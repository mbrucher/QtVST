/**
 * \file QSimpleEQ.cpp
 */

#include <boost/lexical_cast.hpp>

#include <QtGui/QGridLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QMessageBox>
#include <QtGui/QGroupBox>

#include <qevent.h>

#include "QSimpleEQ.h"
#include "simple_eq_effect.h"

float convertGain(int value)
{
  return value / 10.;
}

float convertFrequency(int value)
{
  return 20 * std::pow(10.f, value / 100.f);

}

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
  connect(gain_slider_lmf, SIGNAL(valueChanged(int)), this, SLOT(update_gain_lmf(int)));
  connect(gain_slider_hmf, SIGNAL(valueChanged(int)), this, SLOT(update_gain_hmf(int)));
  connect(gain_slider_hf, SIGNAL(valueChanged(int)), this, SLOT(update_gain_hf(int)));

  connect(cut_slider_lf, SIGNAL(valueChanged(int)), this, SLOT(update_cut_lf(int)));
  connect(cut_slider_lmf, SIGNAL(valueChanged(int)), this, SLOT(update_cut_lmf(int)));
  connect(cut_slider_hmf, SIGNAL(valueChanged(int)), this, SLOT(update_cut_hmf(int)));
  connect(cut_slider_hf, SIGNAL(valueChanged(int)), this, SLOT(update_cut_hf(int)));

  connect(Q_slider_lmf, SIGNAL(valueChanged(int)), this, SLOT(update_Q_lmf(int)));
  connect(Q_slider_hmf, SIGNAL(valueChanged(int)), this, SLOT(update_Q_hmf(int)));

  update_gain_lf(simple_eq->getParameter(0));
  update_gain_lmf(simple_eq->getParameter(1));
  update_gain_hmf(simple_eq->getParameter(2));
  update_gain_hf(simple_eq->getParameter(3));

  update_cut_lf(simple_eq->getParameter(4));
  update_cut_lmf(simple_eq->getParameter(5));
  update_cut_hmf(simple_eq->getParameter(6));
  update_cut_hf(simple_eq->getParameter(7));

  update_Q_lmf(simple_eq->getParameter(8));
  update_Q_hmf(simple_eq->getParameter(9));
}

QWidget* QSimpleEQ::create_LF()
{
  QGroupBox* box = new QGroupBox(this);
  box->setTitle("Low frequency");
  gain_slider_lf = new QtSVGDial;
  gain_slider_lf->setDisplayFunction(convertGain);
  gain_slider_lf->setSkin("Beryl");
  gain_slider_lf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider_lf->setMinimum(-200);
  gain_slider_lf->setMaximum(200);
  gain_slider_lf->updateLabelValue();

  cut_slider_lf = new QtSVGDial;
  cut_slider_lf->setDisplayFunction(convertFrequency);
  cut_slider_lf->setSkin("Beryl");
  cut_slider_lf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  cut_slider_lf->setMinimum(0);
  cut_slider_lf->setMaximum(300);
  cut_slider_lf->updateLabelValue();

  QGridLayout *layout = new QGridLayout(box);
  QLabel* label_gain = new QLabel("Gain");
  layout->addWidget(label_gain, 2, 1, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(gain_slider_lf, 0, 1, 2, 1);
  QLabel* label_cut = new QLabel("Cut (Hz)");
  layout->addWidget(label_cut, 0, 2, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(cut_slider_lf, 1, 2, 2, 1);
  layout->setColumnStretch(0, 1);

  box->setLayout(layout);
  
  return box;
}

QWidget* QSimpleEQ::create_LMF()
{
  QGroupBox* box = new QGroupBox(this);
  box->setTitle("Medium-low frequency");
  Q_slider_lmf = new QtSVGDial;
  Q_slider_lmf->setDisplayFunction(convertGain);
  Q_slider_lmf->setSkin("Beryl");
  Q_slider_lmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  Q_slider_lmf->setMinimum(1);
  Q_slider_lmf->setMaximum(100);
  Q_slider_lmf->updateLabelValue();

  gain_slider_lmf = new QtSVGDial;
  gain_slider_lmf->setDisplayFunction(convertGain);
  gain_slider_lmf->setSkin("Beryl");
  gain_slider_lmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider_lmf->setMinimum(-200);
  gain_slider_lmf->setMaximum(200);
  gain_slider_lmf->updateLabelValue();

  cut_slider_lmf = new QtSVGDial;
  cut_slider_lmf->setDisplayFunction(convertFrequency);
  cut_slider_lmf->setSkin("Beryl");
  cut_slider_lmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  cut_slider_lmf->setMinimum(0);
  cut_slider_lmf->setMaximum(300);
  cut_slider_lmf->updateLabelValue();

  QGridLayout *layout = new QGridLayout(box);
  QLabel* label_Q = new QLabel("Q");
  layout->addWidget(label_Q, 0, 0, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(Q_slider_lmf, 1, 0, 2, 1);
  QLabel* label_gain = new QLabel("Gain");
  layout->addWidget(label_gain, 2, 1, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(gain_slider_lmf, 0, 1, 2, 1);
  QLabel* label_cut = new QLabel("Cut (Hz)");
  layout->addWidget(label_cut, 0, 2, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(cut_slider_lmf, 1, 2, 2, 1);

  box->setLayout(layout);
  
  return box;
}

QWidget* QSimpleEQ::create_HMF()
{
  QGroupBox* box = new QGroupBox(this);
  box->setTitle("Medium-high frequency");
  Q_slider_hmf = new QtSVGDial;
  Q_slider_hmf->setDisplayFunction(convertGain);
  Q_slider_hmf->setSkin("Beryl");
  Q_slider_hmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  Q_slider_hmf->setMinimum(1);
  Q_slider_hmf->setMaximum(100);
  Q_slider_hmf->updateLabelValue();

  gain_slider_hmf = new QtSVGDial;
  gain_slider_hmf->setDisplayFunction(convertGain);
  gain_slider_hmf->setSkin("Beryl");
  gain_slider_hmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider_hmf->setMinimum(-200);
  gain_slider_hmf->setMaximum(200);
  gain_slider_hmf->updateLabelValue();

  cut_slider_hmf = new QtSVGDial;
  cut_slider_hmf->setDisplayFunction(convertFrequency);
  cut_slider_hmf->setSkin("Beryl");
  cut_slider_hmf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  cut_slider_hmf->setMinimum(0);
  cut_slider_hmf->setMaximum(300);
  cut_slider_hmf->updateLabelValue();

  QGridLayout *layout = new QGridLayout(box);
  QLabel* label_Q = new QLabel("Q");
  layout->addWidget(label_Q, 0, 0, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(Q_slider_hmf, 1, 0, 2, 1);
  QLabel* label_gain = new QLabel("Gain");
  layout->addWidget(label_gain, 2, 1, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(gain_slider_hmf, 0, 1, 2, 1);
  QLabel* label_cut = new QLabel("Cut (Hz)");
  layout->addWidget(label_cut, 0, 2, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(cut_slider_hmf, 1, 2, 2, 1);

  box->setLayout(layout);
  
  return box;
}

QWidget* QSimpleEQ::create_HF()
{
  QGroupBox* box = new QGroupBox(this);
  box->setTitle("High frequency");
  gain_slider_hf = new QtSVGDial;
  gain_slider_hf->setDisplayFunction(convertGain);
  gain_slider_hf->setSkin("Beryl");
  gain_slider_hf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider_hf->setMinimum(-200);
  gain_slider_hf->setMaximum(200);
  gain_slider_hf->updateLabelValue();

  cut_slider_hf = new QtSVGDial;
  cut_slider_hf->setDisplayFunction(convertFrequency);
  cut_slider_hf->setSkin("Beryl");
  cut_slider_hf->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  cut_slider_hf->setMinimum(0);
  cut_slider_hf->setMaximum(300);
  cut_slider_hf->updateLabelValue();

  QGridLayout *layout = new QGridLayout(box);
  QLabel* label_gain = new QLabel("Gain");
  layout->addWidget(label_gain, 2, 1, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(gain_slider_hf, 0, 1, 2, 1);
  QLabel* label_cut = new QLabel("Cut (Hz)");
  layout->addWidget(label_cut, 0, 2, Qt::AlignHCenter|Qt::AlignVCenter);
  layout->addWidget(cut_slider_hf, 1, 2, 2, 1);
  layout->setColumnStretch(0, 1);

  box->setLayout(layout);
  
  return box;
}

void QSimpleEQ::update_gain_lf(float value)
{
  int intValue = value * 400 - 200;
  gain_slider_lf->setValue(intValue);  
}

void QSimpleEQ::update_gain_lmf(float value)
{
  int intValue = value * 400 - 200;
  gain_slider_lmf->setValue(intValue);  
}

void QSimpleEQ::update_gain_hmf(float value)
{
  int intValue = value * 400 - 200;
  gain_slider_hmf->setValue(intValue);  
}

void QSimpleEQ::update_gain_hf(float value)
{
  int intValue = value * 400 - 200;
  gain_slider_hf->setValue(intValue);  
}

void QSimpleEQ::update_cut_lf(float value)
{
  int intValue = value * 300 + .5;
  cut_slider_lf->setValue(intValue);  
}

void QSimpleEQ::update_cut_lmf(float value)
{
  int intValue = value * 300 + .5;
  cut_slider_lmf->setValue(intValue);  
}

void QSimpleEQ::update_cut_hmf(float value)
{
  int intValue = value * 300 + .5;
  cut_slider_hmf->setValue(intValue);  
}

void QSimpleEQ::update_cut_hf(float value)
{
  int intValue = value * 300 + .5;
  cut_slider_hf->setValue(intValue);  
}

void QSimpleEQ::update_Q_lmf(float value)
{
  int intValue = value * 100 + .5;
  Q_slider_lmf->setValue(intValue);  
}

void QSimpleEQ::update_Q_hmf(float value)
{
  int intValue = value * 100 + .5;
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
  simple_eq->setParameter(0, (value + 200.) / 400);
}

void QSimpleEQ::update_gain_lmf(int value)
{
  simple_eq->setParameter(1, (value + 200.) / 400);
}

void QSimpleEQ::update_gain_hmf(int value)
{
  simple_eq->setParameter(2, (value + 200.) / 400);
}

void QSimpleEQ::update_gain_hf(int value)
{
  simple_eq->setParameter(3, (value + 200.) / 400);
}

void QSimpleEQ::update_cut_lf(int value)
{
  simple_eq->setParameter(4, value / 300.f);
}

void QSimpleEQ::update_cut_lmf(int value)
{
  simple_eq->setParameter(5, value / 300.f);
}

void QSimpleEQ::update_cut_hmf(int value)
{
  simple_eq->setParameter(6, value / 300.f);
}

void QSimpleEQ::update_cut_hf(int value)
{
  simple_eq->setParameter(7, value / 300.f);
}

void QSimpleEQ::update_Q_lmf(int value)
{
  simple_eq->setParameter(8, value / 100.f);
}

void QSimpleEQ::update_Q_hmf(int value)
{
  simple_eq->setParameter(9, value / 100.f);
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
