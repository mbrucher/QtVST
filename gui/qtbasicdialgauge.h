/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#ifndef QT_BASIC_DIAL_GAUGE_H
#define QT_BASIC_DIAL_GAUGE_H

#include <QtGui/QWidget>

class QSvgRenderer;

class QtBasicDialGauge : public QWidget
{
    Q_OBJECT

public:
    explicit QtBasicDialGauge(QWidget* parent = 0);
    ~QtBasicDialGauge();

    int maximum() const;
    int minimum() const;
    int value() const;

Q_SIGNALS:
    void valueChanged(int value);

public Q_SLOTS:
    void setValue(int value);
    void setMaximum(int maximum);
    void setMinimum(int minimum);
    void setRange(int minimum, int maximum);

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual QSize sizeHint() const;

    void init();

private:
    int m_value;
    int m_minimum;
    int m_maximum;
};

#endif // QT_BASIC_DIAL_GAUGE_H
