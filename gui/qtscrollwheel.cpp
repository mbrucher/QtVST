/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include "qtscrollwheel.h"

#include <QtGui/QDesktopWidget>
#include <QtCore/QDebug>
#include <QtGui/QPainter>
#include <QSvgRenderer>
#include <QtGui/QMouseEvent>
#include <QApplication>

/*!

    \class QtScrollWheel qtscrollwheel.h

    The description of the QtScrollWheel.

    \brief The QtScrollWheel class provides a scroll wheel to set values.

    The QtScrollWheel is an example to show the capabilities of the Qt Framework
    related to customized controls.

    Here is a possible implementation shown

    \code

        // Create a QtScrollWheel
        QWidget * widget = new QWidget(this)
        QtScrollWheel * wheel = new QtScrollWheel(widget);
        wheel->setSkin("Beryl");
        widget->addWidget(wheel);
        widget->show();

    \endcode

    It is important to call the function \a setSkin() to load a skin. If \a setSkin() is not called
    the QtScrollWheel will not have any visible content.
    The parameter \p skin has the skin name. The skins are in the resource file defined in the project file.
    The name scheme in the resource file is "Skin Name"/"unique picture name.svg".

    The \a setSkin() function loads the diffrent skins over the "Skin name".
    The needed realised graphics for this control are "normal.svg", "hovered.svg" and "pressed.svg".
    If you want to add new skins, they have to follow the name scheme to work.

    The actual skin can be read over the \a skin() function.

    \sa skin(), setSkin()


    No Signals defined. \n

    No Slots defined.


*/

/*!
    Constructor of the QAbstractSlider with \p parent as
    Parent.
*/
QtScrollWheel::QtScrollWheel(QWidget* parent)
    : QAbstractSlider(parent)
{
    init();
}

/*!
    Destructor
*/
QtScrollWheel::~QtScrollWheel()
{
}

/*!
    \internal
    Internal initialisation function.
*/
void QtScrollWheel::init()
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_currentIndex = 0;
}

/*!
    With this function you can set the skin that will be displayed in the widget.

    \code
        QtScrollWheel * scroll = new QtScrollWheel(this);
        scroll->setSkin("Beryl");
    \endcode

    This function has to be called before using the QtScrollWheel.

    \sa skin()

*/
void QtScrollWheel::setSkin(const QString& skin)
{
    m_skin = skin;
    const QString base = ":/scrollwheel/" + skin + '/';

    for (int i = 0; i < 3; ++i) {
        m_wheel[i].load(base + QString("wheel_%1.svg").arg(i+1));

    }

    // update geometry for new sizeHint and repaint
    updateGeometry();
    update();
}

/*!

    This function returns the actual skin name. If no skin has been set the return value is "".

    \code
        QtScrollWheel * scroll = new QtScrollWheel(this);
        scroll->setSkin("Beryl");
        qDebug() << scroll->skin();
    \endcode

    \sa setSkin()

*/
QString QtScrollWheel::skin() const
{
    return m_skin;
}

/*!
    \overload
    \internal
    Overloaded mouse move event. Calculate new position of graphic.
*/
void QtScrollWheel::mouseMoveEvent(QMouseEvent* event)
{
    int diff = event->pos().y() - m_lastMousePosition.y();
    if (qAbs(diff) > QApplication::startDragDistance()) {
        changeValue((diff > 0) ? -1 : 1);
        m_lastMousePosition = event->pos();
    }
}

/*!
    \overload
    \internal
    Overloaded mouse press event. Save last mouse position.
*/
void QtScrollWheel::mousePressEvent(QMouseEvent* event)
{
    // remember mouse position for mouse move event
    m_lastMousePosition = event->pos();
    QAbstractSlider::mousePressEvent(event);
}

/*!
    \overload
    \internal
    Overloaded wheel event. Change position of wheel over mouse wheel.
*/
void QtScrollWheel::wheelEvent(QWheelEvent* event)
{
    const int diff = QApplication::wheelScrollLines();
    changeValue(event->delta() > 0 ? diff : -diff);
}

/*!
    \internal
    Change value to actual value added with \p delta.
*/
void QtScrollWheel::changeValue(int delta)
{
    if (delta < 0 && minimum() < value()) {
        m_currentIndex = (m_currentIndex + 2) % 3; // -1 (decrease)
        setValue(value() + delta);
        update();
    } else if (delta > 0 && maximum() > value()) {
        m_currentIndex = (m_currentIndex + 1) % 3; // +1 (increase)
        setValue(value() + delta);
        update();
    }
}

/*!
    \internal
    \overload
    Overloaded paint event. Draw wheel graphic.
*/
void QtScrollWheel::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    m_wheel[m_currentIndex].render(&painter, QRectF(0,0, width(), height()));
}
/*!
    \internal
    \overload
    Set size hint. Can be changed at runtime.
*/
QSize QtScrollWheel::sizeHint() const
{
    return QSize(80,200);

   if (!m_defaultSize.isNull()) {
        double factor = static_cast<double>(m_defaultSize.height()) / m_defaultSize.width();
        return QSize(70, factor * 70);
    } else {
        return QAbstractSlider::sizeHint();
    }
}
