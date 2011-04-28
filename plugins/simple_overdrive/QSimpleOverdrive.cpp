/**
 * \file QSimpleOverdrive.cpp
 */

#include <boost/lexical_cast.hpp>

#include <QtGui/QGridLayout>

#include "QSimpleOverdrive.h"
#include "simple_overdrive_effect.h"

QSimpleOverdrive::QSimpleOverdrive(SimpleOverdriveEffect* simple_overdrive, HWND h_parent)
:QWinWidget(h_parent, NULL), simple_overdrive(simple_overdrive), h_parent(h_parent), sample_rate(simple_overdrive->getSampleRate())
{
  setAttribute(Qt::WA_DeleteOnClose);

  QLabel *gain_label = new QLabel("Gain", this);
  gain_slider = new QtSVGDial(this);
  gain_slider->setSkin("Beryl");
  gain_slider->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  gain_slider->setMinimum(-40);
  gain_slider->setMaximum(40);
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
  
  connect(gain_slider, SIGNAL(valueChanged(int)), this, SLOT(update_gain(int)));
  connect(type_combo, SIGNAL(activated(int)), this, SLOT(update_oversampling_log(int)));
  
  update_gain(simple_overdrive->getParameter(0));
}

void QSimpleOverdrive::update_gain(int value)
{
  simple_overdrive->setParameter(0, std::pow(10, value / 20.));
}

void QSimpleOverdrive::update_gain(float value)
{
  int intValue = static_cast<int>(std::log(value) / std::log(10.f) * 20);
  gain_slider->setValue(intValue);  
}

void QSimpleOverdrive::update_oversampling_log(int value)
{
  simple_overdrive->set_oversampling(1 << (value + 1));
}

void QSimpleOverdrive::update_oversampling(int value)
{
  int oversampling = -2;
  while(value != 0)
  {
    value = value >> 1;
    ++oversampling;
  }
  type_combo->setCurrentIndex(oversampling);
}
