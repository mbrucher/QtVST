/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include "qtsvgdial.h"

#include <QDesktopWidget>
#include <QtCore/QDebug>
#include <QPainter>
#include <QSvgRenderer>
#include <qevent.h>
#include "QtBasicDialGauge"

#include <QLabel>
#include <QVBoxLayout>

#include <qmath.h>
#include <private/qmath_p.h>

/*!
    \class QtSVGDial QtSVGDial.h

    \brief The QtSVGDial provides a scroll widget with SVG graphics.

    QtSVGDial is a custom widget and behaves like a slider.

    \section1 Example
    \code
    QtSVGDial * scroll = new QtSVGDial(parent);
    scroll->setSkin("Beryl");
    scroll->setRange(0, 100);
    scroll->setValue(50);
    \endcode

    The function \a minimum() and \a maximum() are used from the QAbstractSlider.

    It is important to call the function \a setSkin() to load a skin. If \a setSkin() is not called
    the QtSVGDial will not have any visible content.
    The parameter \p skin has the skin name. The skins are in the resource file defined in the project file.
    The name scheme in the resource file is "Skin Name"/"unique picture name.svg".

    The \a setSkin() function loads the diffrent skins over the "Skin name".
    The needed realised graphics for this control are "normal.svg", "hovered.svg" and "pressed.svg".
    If you want to add new skins, they have to follow the name scheme to work.

    The actual skin can be read over the \a skin() function.

    \sa skin(), setSkin()
*/

/*!
    Constructor of the QAbstractSlider with \p parent as
    Parent.
*/
QtSVGDial::QtSVGDial(QWidget* parent)
    : QAbstractSlider(parent), function(NULL)
{
    init();
}

/*!
    Destructor
*/
QtSVGDial::~QtSVGDial()
{

}

/*!
    \internal
    Internal initialisation function.
*/
void QtSVGDial::init()
{
    // set hover attribute so a repaint event is triggered on hover
    setAttribute(Qt::WA_Hover, true);

    // create the gui elements for the scroll dial: gauge + label
    QVBoxLayout* vLayout = new QVBoxLayout(this);
    vLayout->setSpacing(0);
    m_dialGauge = new QtBasicDialGauge(this);
    m_dialGauge->setRange(minimum(), maximum());
    m_dialGauge->setValue(value());
    m_label = new QLabel("0", this);
    m_label->setAlignment(Qt::AlignHCenter);
    m_label->setMinimumWidth(35);
    m_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    vLayout->addWidget(m_dialGauge, 0, Qt::AlignHCenter);
    vLayout->addWidget(m_label, 0, Qt::AlignHCenter);

    // make sure this slider and the gauge are in sync
    connect(this, SIGNAL(valueChanged(int)), m_dialGauge, SLOT(setValue(int)));
    connect(this, SIGNAL(rangeChanged(int, int)), m_dialGauge, SLOT(setRange(int, int)));

    // make sure the label shows the correct value
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateLabelValue()));
}

/*!
    With this function you can set the skin that will be displayed in the widget.

    \code
        QtSVGDial * scroll = new QtSVGDial(this);
        scroll->setSkin("Beryl");
    \endcode

    This function has to be called before using the QtSVGDial.

    \sa skin()

*/
void QtSVGDial::setSkin(const QString& skin)
{
    m_skin = skin;
    const QString base = ":/scrolldial/" + skin + '/';

    m_label->setStyleSheet("color: white; border-width: 2px;"
                           "border-image: url(" + base + "label.svg);");

    // set to null pictures
    m_background = QPicture();
    m_hoverBackground = QPicture();

    QSvgRenderer renderer;
    QPainter painter;

    if (renderer.load(base + "background.svg")) {
        painter.begin(&m_background);
        renderer.render(&painter, QRectF(0.0, 0.0, 1.0, 1.0));
        painter.end();
    }

    if (renderer.load(base + "background_hover.svg")) {
        painter.begin(&m_hoverBackground);
        renderer.render(&painter, QRectF(0.0, 0.0, 1.0, 1.0));
        painter.end();
    }

    // update geometry for new sizeHint and repaint
    updateGeometry();
    update();
}

/*!

    This function returns the actual skin name. If no skin has been set the return value is "".

    \code
        QtSVGDial * scroll = new QtSVGDial(this);
        scroll->setSkin("Beryl");
        qDebug() << scroll->skin();
    \endcode

    \sa setSkin()

*/
QString QtSVGDial::skin() const
{
    return m_skin;
}

void QtSVGDial::mousePressEvent(QMouseEvent *e)
{
    if (maximum() == minimum() ||
        (e->button() != Qt::LeftButton)  ||
        (e->buttons() ^ e->button())) {
        e->ignore();
        return;
    }
    e->accept();
    setSliderPosition(valueFromPoint(e->pos()));
    // ### This isn't quite right,
    // we should be doing a hit test and only setting this if it's
    // the actual dial thingie (similar to what QSlider does), but we have no
    // subControls for QtSVGDial.
    setSliderDown(true);
}

int QtSVGDial::valueFromPoint(const QPoint &p) const
{
    double yy = (double)height()/2.0 - p.y();
    double xx = (double)p.x() - width()/2.0;
    double a = (xx || yy) ? qAtan2(yy, xx) : 0;

    if (a < Q_PI / -2)
        a = a + Q_PI * 2;

    int dist = 0;
    int minv = minimum(), maxv = maximum();

    if (minimum() < 0) {
        dist = -minimum();
        minv = 0;
        maxv = maximum() + dist;
    }

    int r = maxv - minv;
    int v;
    v =  (int)(0.5 + minv + r* (Q_PI * 4 / 3 - a) / (Q_PI * 10 / 6));

    if (dist > 0)
        v -= dist;

    return bound(v);
}

/*!
  \reimp
*/

void QtSVGDial::mouseReleaseEvent(QMouseEvent * e)
{
    if (e->buttons() & (~e->button()) ||
       (e->button() != Qt::LeftButton)) {
        e->ignore();
        return;
    }
    e->accept();
    setValue(valueFromPoint(e->pos()));
    setSliderDown(false);
}


/*!
  \reimp
*/

void QtSVGDial::mouseMoveEvent(QMouseEvent * e)
{
    if (!(e->buttons() & Qt::LeftButton)) {
        e->ignore();
        return;
    }
    e->accept();
    setSliderPosition(valueFromPoint(e->pos()));
}

/*!
    \overload
    \internal
    Draws the background.
    Event \a ev not used.
*/
void QtSVGDial::paintEvent(QPaintEvent* ev)
{
    Q_UNUSED(ev);
    QPainter p(this);
    p.save();
    p.scale(width(), height());
    if (underMouse()) {
        m_hoverBackground.play(&p);
    } else {
        m_background.play(&p);
    }
    p.restore();
}

/**
    \internal
    Updates the label content.
**/
void QtSVGDial::updateLabelValue()
{
    if(function == NULL)
	{
      m_label->setText(QString::number(value()));
	}
	else
	{
	  m_label->setText(QString::number((*function)(value())));
	}
}

void QtSVGDial::setDisplayFunction(DisplayFunction function)
{
    this->function = function;
}
