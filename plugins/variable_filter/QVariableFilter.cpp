/**
 * \file QVariableFilter.cpp
 */

#include <boost/lexical_cast.hpp>

#include <QtGui/QGridLayout>

#include "QVariableFilter.h"
#include "variable_filter_effect.h"

QVariableFilter::QVariableFilter(AudioEffectX *variable_filter, HWND h_parent)
:QWinWidget(h_parent, NULL), variable_filter(variable_filter), h_parent(h_parent), sample_rate(variable_filter->getSampleRate())
{
  setAttribute(Qt::WA_DeleteOnClose);

  QLabel *Flabel = new QLabel("Cutoff", this);
  frequency_label = new QLabel("0", this);
  frequency_label->setMinimumWidth(50);
  frequency_slider = new QSlider(Qt::Horizontal, this);
  frequency_slider->setMinimumWidth(300);
  QLabel *Qlabel = new QLabel("Q", this);
  q_label = new QLabel("0", this);
  q_slider = new QSlider(Qt::Horizontal, this);
  QLabel *Slabel = new QLabel("Selection", this);
  type_combo = new QComboBox(this);
  type_combo->addItem("Low-pass");
  type_combo->addItem("Band-pass");
  type_combo->addItem("High-pass");

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(Flabel, 0, 0);
  layout->addWidget(frequency_label, 0, 1);
  layout->addWidget(frequency_slider, 0, 2);
  layout->addWidget(Qlabel, 1, 0);
  layout->addWidget(q_label, 1, 1);
  layout->addWidget(q_slider, 1, 2);
  layout->addWidget(Slabel, 2, 0);
  layout->addWidget(type_combo, 2, 1, 1, 2);

  setLayout(layout);
  
  connect(frequency_slider, SIGNAL(valueChanged(int)), this, SLOT(update_frequency(int)));
  connect(q_slider, SIGNAL(valueChanged(int)), this, SLOT(update_q(int)));
  connect(type_combo, SIGNAL(activated(int)), this, SLOT(update_type(int)));
}

void QVariableFilter::update_frequency(int value)
{
  variable_filter->setParameter(0, std::pow(10, value / 100.) / 10);
}

void QVariableFilter::update_frequency(float value)
{
  int intValue = static_cast<int>(std::log(value * 10.) / std::log(10.) * 100);
  frequency_label->setText(QString::number(value * sample_rate / 2));
  frequency_slider->setValue(intValue);  
}

void QVariableFilter::update_q(int value)
{
  variable_filter->setParameter(1, value / 100.);
}

void QVariableFilter::update_q(float value)
{
  int intValue = static_cast<int>(value * 100);
  q_label->setText(QString::number(value));
  q_slider->setValue(intValue);  
}

void QVariableFilter::update_type(int value)
{
  variable_filter->setParameter(2, value);
}
