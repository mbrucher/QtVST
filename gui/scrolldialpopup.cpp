/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include "scrolldialpopup.h"
#include "QtScrollDial"

#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QSvgRenderer>
#include <QtGui/QVBoxLayout>

#include "QtBasicDialGauge"
#include "QtScrollWheel"
#include <QtGui/QLabel>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtGui/QBitmap>
#include <QtGui/QPixmap>
/*!

    \class ScrollDialPopup scrolldialpopup.h

    The description of the QtBasicDialGauge.

    \brief The ScrollDialPopup is a private class of QtScrollDial.
*/

/*!
    Constructor of the ScrollDialPopup with \p parent as
    parent.
*/
ScrollDialPopup::ScrollDialPopup(QtScrollDial* parent)
    : QWidget(parent)
    , m_scrollDial(parent)
{
    init();
}

/*!
    Destructor
*/
ScrollDialPopup::~ScrollDialPopup()
{
}

/*!
    \internal
    Function that initalize variables.
*/
void ScrollDialPopup::init()
{
    setStyleSheet("background-color: rgb(128,128,128);\n");

    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(0);
    vLayout->setContentsMargins(30, 30, 30, 8);
    m_gauge = new QtBasicDialGauge(this);
    m_gauge->setFixedSize(60, 60);
    m_label = new QLabel("0", this);
    m_label->setAlignment(Qt::AlignHCenter);
    m_label->setMinimumWidth(40);
    m_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    m_wheel = new QtScrollWheel(this);

    m_closeButton = new PopupCloseButton(this);
    m_closeButton->setFocusPolicy(Qt::NoFocus);
    m_closeButton->setFixedSize(50, 50);
    m_closeButton->raise();

    vLayout->addSpacing(10); // makes room for close button
    vLayout->addWidget(m_gauge, 0, Qt::AlignHCenter);
    vLayout->addWidget(m_label, 0, Qt::AlignHCenter);
    vLayout->addSpacing(4);
    vLayout->addWidget(m_wheel, 0, Qt::AlignHCenter);

    connect(m_wheel, SIGNAL(valueChanged(int)), this, SLOT(updateLabelValue()));
    connect(m_wheel, SIGNAL(valueChanged(int)), m_gauge, SLOT(setValue(int)));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(hide()));
    connect(m_closeButton, SIGNAL(clicked()), this, SLOT(commitValue()));

    m_preventInfiniteLoop = false;
}

/*!
    With this function you can set the skin that will be displayed in the widget.

    \code
        ScrollDialPopup * scroll = new ScrollDialPopup(this);
        scroll->setSkin("Beryl");
    \endcode

    This function has to be called before using the ScrollDialPopup.

    \sa skin()

*/
void ScrollDialPopup::setSkin(const QString& skin)
{
    m_skin = skin;
    const QString base = ":/scrolldial/" + skin + '/';

    // m_gauge does not support setSkin(), it's painted with QPainter
    m_wheel->setSkin(skin);
    m_label->setStyleSheet(QString("font: %1pt;").arg(font().pointSize() + 2) +
                           "color: white; border-width: 2px;"
                           "border-image: url(" + base + "label.svg);");
    m_closeButton->setSkin(skin);

    // set null pictures
    m_backgroundPicture = QPicture();

    QSvgRenderer renderer;
    QPainter painter;

    if (renderer.load(base + "popup_background.svg")) {
        painter.begin(&m_backgroundPicture);
        renderer.render(&painter, QRectF(0.0, 0.0, 1.0, 1.0));
        painter.end();
    }
}

/*!

    This function returns the actual skin name. If no skin has been set the return value is "".

    \code
        ScrollDialPopup * scroll = new ScrollDialPopup(this);
        scroll->setSkin("Beryl");
        qDebug() << scroll->skin();
    \endcode

    \sa setSkin()

*/
QString ScrollDialPopup::skin() const
{
    return m_skin;
}

/*!
Sets the ScrollDialPopup's minimum to min and its maximum to max.
If max is smaller than min, min becomes the only legal value.
See also minimum and maximum.
*/
void ScrollDialPopup::setRange(int minimum, int maximum)
{
    m_wheel->setRange(minimum, maximum);
    m_gauge->setRange(minimum, maximum);
}
/*!
    Set the current value of the display and wheel widget.
*/
void ScrollDialPopup::setValue(int value)
{
    m_wheel->setValue(value);
    m_gauge->setValue(value);
    updateLabelValue();
}

/*!
    Update the label to the current value.
*/
void ScrollDialPopup::updateLabelValue()
{
    m_label->setText(QString::number(m_wheel->value()));
}

/*!
    \overload
    \internal
    Overloaded function to update value on hide.
*/
void ScrollDialPopup::hideEvent(QHideEvent* event)
{
    if (event->spontaneous()) {
        return;
    }

    m_scrollDial->update();
}

/*!
    \overload
    \internal
    Overloaded function to calculate geometry of CloseButton.
*/
void ScrollDialPopup::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    if (m_preventInfiniteLoop) {
        return;
    }
    m_preventInfiniteLoop = true;

    const int w = m_closeButton->width();
    m_closeButton->setGeometry(event->size().width() - w, 0, w, w);
    updateMask();
    m_preventInfiniteLoop = false;
}

/*!
    \overload
    \internal
    Overloaded function to update the mask.
*/
void ScrollDialPopup::updateMask()
{
    QPixmap pixmap(size());
    pixmap.fill(Qt::transparent);

    const double spacing = m_closeButton->width() / 2.0 - 1;

    QPainter painter(&pixmap);
    painter.initFrom(this);

    painter.translate(spacing, spacing);
    painter.scale(width() - 2 * spacing, height() - spacing);

    m_backgroundPicture.play(&painter);

    painter.end();
    m_closeButton->render(&pixmap, QPoint(width() - m_closeButton->width(), 0),
                          QRegion(), RenderFlags(DrawChildren));

    QBitmap bitmap = pixmap.createHeuristicMask();
    setMask(bitmap);
}

/*!
    \overload
    \internal
    Overloaded function to update the mask.
*/
void ScrollDialPopup::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    const double spacing = m_closeButton->width() / 2.0 - 1;

    QPainter painter(this);
    painter.translate(spacing, spacing);
    painter.scale(width() - 2 * spacing, height() - spacing);

    m_backgroundPicture.play(&painter);
}

/*!
    \overload
    \internal
    Use wheel event.
*/
void ScrollDialPopup::wheelEvent(QWheelEvent* event)
{
    // forward wheel event to wheel
    QCoreApplication::sendEvent(m_wheel, event);
}

void ScrollDialPopup::commitValue()
{
    m_scrollDial->setValue(m_wheel->value());
    m_scrollDial->update();
}

/*!
    \class PopupCloseButton scrolldialpopup.h
    \brief Private class used in ScrollDialPopup
*/
PopupCloseButton::PopupCloseButton(QWidget* parent)
    : QAbstractButton(parent)
{
    // set hover attribute so a repaint event is triggered on hover
    setAttribute(Qt::WA_Hover, true);
}

PopupCloseButton::~PopupCloseButton()
{
}

void PopupCloseButton::setSkin(const QString& skin)
{
    m_skin = skin;
    const QString base = ":/scrolldial/" + skin + '/';

    // set null pictures
    m_normal = QPicture();
    m_hovered = QPicture();
    m_pressed = QPicture();

    QSvgRenderer renderer;
    QPainter painter;

    if (renderer.load(base + "closebutton_normal.svg")) {
        painter.begin(&m_normal);
        renderer.render(&painter, QRectF(0.0, 0.0, 1.0, 1.0));
        painter.end();
    }
    if (renderer.load(base + "closebutton_hovered.svg")) {
        painter.begin(&m_hovered);
        renderer.render(&painter, QRectF(0.0, 0.0, 1.0, 1.0));
        painter.end();
    }
    if (renderer.load(base + "closebutton_pressed.svg")) {
        painter.begin(&m_pressed);
        renderer.render(&painter, QRectF(0.0, 0.0, 1.0, 1.0));
        painter.end();
    }

    // update geometry for new sizeHint and repaint
    updateGeometry();
    update();

}

QString PopupCloseButton::skin() const
{
    return m_skin;
}

void PopupCloseButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.scale(width(), height());

    if (isDown()) {
        m_pressed.play(&painter);
    } else if (underMouse()) {
        m_hovered.play(&painter);
    } else {
        m_normal.play(&painter);
    }
}


