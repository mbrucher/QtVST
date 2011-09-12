/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#ifndef QT_SCROLL_WHEEL_H
#define QT_SCROLL_WHEEL_H

#include <QAbstractSlider>
#include "qtsvgpixmapcache.h"
          
class QMouseEvent;

class QtScrollWheel : public QAbstractSlider
{
    Q_OBJECT
    Q_PROPERTY(QString skin READ skin WRITE setSkin)
public:
    QtScrollWheel(QWidget* parent = NULL);
    ~QtScrollWheel();

    void setSkin(const QString& skin);
    QString skin() const;
protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual QSize sizeHint() const;

    void init();
    void changeValue(int delta);

private:
    /** loaded graphics **/
    QtSvgPixmapCache m_wheel[3];
    /** default size of SVG graphic **/
    QSize m_defaultSize;
    /** actual shown picture 0-2 **/
    int m_currentIndex;
    QPoint m_lastMousePosition;
    /** actual skin name **/
    QString m_skin;
};

#endif // QT_SCROLL_WHEEL_H
