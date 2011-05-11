/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#ifndef QT_SVG_DIAL_H
#define QT_SVG_DIAL_H

#include <QtGui/QAbstractSlider>
#include <QPointer>
#include <QPicture>
#include <QtCore/QString>

class QtBasicDialGauge;
class QLabel;

class QtSVGDial : public QAbstractSlider
{
    Q_OBJECT
    Q_PROPERTY(QString skin READ skin WRITE setSkin)
public:
    QtSVGDial(QWidget* parent = NULL);
    ~QtSVGDial();

    void setSkin(const QString& skin);
    QString skin() const;
	
	typedef float (*DisplayFunction)(int value);

	void setDisplayFunction(DisplayFunction function);
	
private Q_SLOTS:
    void updateLabelValue();

protected:
    void mousePressEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
//    virtual void mousePressEvent(QMouseEvent* ev);
    virtual void paintEvent(QPaintEvent* ev);

    void init();

	DisplayFunction function;
	
private:
    int valueFromPoint(const QPoint &p) const;
    inline int bound(int val) const { return qMax(minimum(), qMin(maximum(), val)); }

    QPicture m_background;
    QPicture m_hoverBackground;

    QtBasicDialGauge* m_dialGauge;
    QLabel* m_label;
    /** actual skin name **/
    QString m_skin;
};

#endif // QT_SVG_DIAL_H
