/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#ifndef QT_SCROLL_DIAL_H
#define QT_SCROLL_DIAL_H

#include <QtGui/QAbstractSlider>
#include <QPointer>
#include <QPicture>
#include <QtCore/QString>

#ifdef FILTERGUIEXPORT
# define FILTERGUIEXPORT_ __declspec(dllexport)
#else
# define FILTERGUIEXPORT_ __declspec(dllimport)
#endif

class ScrollDialPopup;
class QtBasicDialGauge;
class QLabel;

class FILTERGUIEXPORT_ QtScrollDial : public QAbstractSlider
{
    Q_OBJECT
    Q_PROPERTY(QString skin READ skin WRITE setSkin)
public:
    QtScrollDial(QWidget* parent = NULL);
    ~QtScrollDial();

    void setSkin(const QString& skin);
    QString skin() const;

private Q_SLOTS:
    void updateLabelValue();

protected:
    virtual void mousePressEvent(QMouseEvent* ev);
    virtual void paintEvent(QPaintEvent* ev);

    QPoint popupPosition();
    void init();

private:
    ScrollDialPopup* m_popup;
    QPicture m_background;
    QPicture m_hoverBackground;

    QtBasicDialGauge* m_dialGauge;
    QLabel* m_label;
    /** actual skin name **/
    QString m_skin;
};

#endif // QT_SCROLL_DIAL_H
