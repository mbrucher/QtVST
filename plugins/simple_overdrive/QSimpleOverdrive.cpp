/**
 * \file QSimpleOverdrive.cpp
 */

#include <boost/lexical_cast.hpp>

#include <QtGui/QGridLayout>

#include "QSimpleOverdrive.h"
#include "simple_overdrive_effect.h"

QSimpleOverdrive::QSimpleOverdrive(AudioEffectX *simple_overdrive, HWND h_parent)
:QWinWidget(h_parent, NULL), simple_overdrive(simple_overdrive), h_parent(h_parent), sample_rate(simple_overdrive->getSampleRate())
{
  setAttribute(Qt::WA_DeleteOnClose);

  QLabel *gain_label_1 = new QLabel("Gain", this);
  gain_label = new QLabel("0", this);
  gain_label->setMinimumWidth(50);
  gain_slider = new QSlider(Qt::Horizontal, this);
  gain_slider->setMinimumWidth(300);
  QLabel* type_label_1 = new QLabel("Oversampling", this);
  type_combo = new QComboBox(this);
  type_combo->addItem("2");
  type_combo->addItem("4");
  type_combo->addItem("8");
  type_combo->addItem("16");
  type_combo->addItem("32");

  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(gain_label_1, 0, 0);
  layout->addWidget(gain_label, 0, 1);
  layout->addWidget(gain_slider, 0, 2);
  layout->addWidget(type_label_1, 1, 0);
  layout->addWidget(type_combo, 1, 1, 1, 2);

  setLayout(layout);
  
  connect(gain_slider, SIGNAL(valueChanged(int)), this, SLOT(update_gain(int)));
  connect(type_combo, SIGNAL(activated(int)), this, SLOT(update_type(int)));
}

void QSimpleOverdrive::update_gain(int value)
{
  simple_overdrive->setParameter(0, std::pow(10, value / 20.));
}

void QSimpleOverdrive::update_gain(float value)
{
  int intValue = static_cast<int>(std::log(value) * 20);
  gain_label->setText(QString::number(value));
  gain_slider->setValue(intValue);  
}

void QSimpleOverdrive::update_oversampling(int value)
{
}
