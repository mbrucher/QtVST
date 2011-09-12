/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#ifndef QT_SCROLL_DIAL_POPUP_H
#define QT_SCROLL_DIAL_POPUP_H

#include <QtGui/QWidget>
#include <QtGui/QPicture>
#include <QtCore/QString>

#include <QtGui/QAbstractButton>

class QtScrollDial;

class QtScrollWheel;
class QtBasicDialGauge;
class QLabel;


class PopupCloseButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QString skin READ skin WRITE setSkin)
public:
    PopupCloseButton(QWidget* parent = 0);
    virtual ~PopupCloseButton();

    void setSkin(const QString& skin);
    QString skin() const;
protected:
    virtual void paintEvent(QPaintEvent* event);

    QPicture m_normal;
    QPicture m_hovered;
    QPicture m_pressed;
private:
    QString m_skin;
};

class ScrollDialPopup : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString skin READ skin WRITE setSkin)
public:
    ScrollDialPopup(QtScrollDial* parent);
    virtual ~ScrollDialPopup();

    void setSkin(const QString& skin);
    QString skin() const;

    void setRange(int minimum, int maximum);
    void setValue(int value);

private Q_SLOTS:
    void updateLabelValue();
    void commitValue();

protected:
    virtual void hideEvent(QHideEvent* event);
    virtual void resizeEvent(QResizeEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

    void init();
    void updateMask();

private:
    QtScrollDial* m_scrollDial;
    QtScrollWheel* m_wheel;
    QLabel* m_label;
    QtBasicDialGauge* m_gauge;
    PopupCloseButton* m_closeButton;

    QPicture m_backgroundPicture;
    /** actual skin name **/
    QString m_skin;
    /** to avoid endless loop and resize only once **/
    bool m_preventInfiniteLoop;
};

#endif // QT_SCROLL_DIAL_POPUP_H
