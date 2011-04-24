/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include <QtSvg/QSvgRenderer>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#include <cmath>

#include <QtCore/QDebug>

#include "qtbasicdialgauge.h"

/*!

    \class QtBasicDialGauge qtbasicdialgauge.h

    The description of the QtBasicDialGauge.

    \brief The QtBasicDialGauge class provides a basic dial gauge with SVG
    graphics.

    The QtBasicDialGauge is an example to show the capabilities of the Qt Framework
    related to customized controls.

    Here is a possible implementation shown

    \code

        // Create a QtSvgDialGauge
        QWidget * widget = new QWidget(this)
        QtBasicDialGauge * gauge = new QtBasicDialGauge(widget);
        gauge->setRange(0, 100);
        gauge->setValue(0):
        widget->addWidget(gauge);
        widget->show();

    \endcode

    The signal \a valueChanged() is emitted when the QtBasicDialGauge value
    has changed, with the new value as argument.

    The most useful functions are \a setValue() to set the QtBasicDialGauge directly
    to some value. \n
    With the function \a setRange() it is possible to set the smallest
    and biggest visible value.The values can be set seperatly with
    \a setMinimum() and \a setMaximum(). The actual minimum and maximum value
    can be read over the \a minimum() and \a maximum() function.

    Signals
    \fn     void valueChanged(int value)

    Slots
    \fn void setValue(int value)
    \fn void setMaximum(int maximum)
    \fn void setMinimum(int minimum)
    \fn void setRange(int minimum, int maximum)

*/

/*!
    Constructor of the widget with \p parent as
    Parent.
*/
QtBasicDialGauge::QtBasicDialGauge(QWidget * parent)
    : QWidget(parent)
{
    init();
    update();
}

/*!
    Destructor
*/
QtBasicDialGauge::~QtBasicDialGauge()
{
}

/*!
    \internal
    Private initialization function.
*/
void QtBasicDialGauge::init()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_value = 0;
    m_minimum = 0;
    m_maximum = 100;
}

/*!
    \internal
    \overload
    Overloaded paint event to draw the QtBasicDialGauge components.
*/
void QtBasicDialGauge::paintEvent(QPaintEvent * event)
{
    Q_UNUSED(event);

    // generate the target rect
    const int spacing = 5;
    QRectF contentRect = rect().adjusted(spacing, spacing, -spacing, -spacing);

    // attach painter to paint devicer (widget)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // generate a white pen
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(1.0 + width() / 15.0); // scale the pen depending on the widget width
    pen.setColor(Qt::white);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);


    // finally draw the arc, see QPainter::drawArc for details
    painter.drawArc(contentRect, (-45)*16, (2*135)*16);

    // draw the value indicator
    double valueInPercent = static_cast<double>(m_value - m_minimum) / (m_maximum - m_minimum);

    const double math_pi = 3.14159265358979323846; // M_PI is not defined on every platform
    double degree = (135 + 90) - valueInPercent * 2.0 * 135.0;
    degree = degree * math_pi / 180.0;
    double radius = (contentRect.width() - spacing * 0.5) * 0.5;
    QPointF vec(radius * cos(degree),
                radius * -sin(degree));
    painter.drawLine(contentRect.center(), contentRect.center() + vec);
}

/*!
    \internal
    \overload
    Set default size hint. Can be changed at runtime.
*/
QSize QtBasicDialGauge::sizeHint() const
{
    return QSize(50, 50);
}

/*!
    Returns the set maximum value.
*/
int QtBasicDialGauge::maximum() const
{
    return m_maximum;
}

/*!
    Returns the set minimum value.
    \sa maximum()
*/
int QtBasicDialGauge::minimum() const
{
    return m_minimum;
}

/*!
    Returns the actual value.
    \sa setValue()
*/
int QtBasicDialGauge::value() const
{
    return m_value;
}

/*!
    Sets the actual value if the value is >= minimum() and
    value is <= maximum().
    \sa minimum(), maximum()
*/
void QtBasicDialGauge::setValue(int value)
{
    if (value >= m_minimum && value <= m_maximum) {
        m_value = value;
        emit valueChanged(value);
        update();
    }
}

/*!
    Sets the maximum value if maximum is > minimum()
    \sa minimum()
*/
void QtBasicDialGauge::setMaximum(int maximum)
{
    m_maximum = maximum;
    if (m_maximum < m_minimum) {
        m_minimum = m_maximum;
    }
    update();
}

/*!
    Sets the minimum value if minimum is > maximum()
    \sa minimum()
*/
void QtBasicDialGauge::setMinimum(int minimum)
{
    m_minimum = minimum;
    if (m_minimum > m_maximum) {
        m_maximum = m_minimum;
    }
    update();
}

/*!
    Sets the minimum and maximum value if minimum < maximum
    \sa minimum()
*/
void QtBasicDialGauge::setRange(int minimum, int maximum)
{
    if(minimum < maximum)
    {
        setMinimum(minimum);
        setMaximum(maximum);
    }
}
