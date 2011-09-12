/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/


#ifndef QT_SVG_PIXMAP_CACHE
#define QT_SVG_PIXMAP_CACHE

#include <QtCore/QSizeF>
#include <QObject>


class QPainter;
class QString;
class QRectF;

class QtSvgPixmapCachePrivate;

class QtSvgPixmapCache : public QObject
{
public:
    QtSvgPixmapCache(QObject* parent = NULL);
    QtSvgPixmapCache(const QString& url);
    ~QtSvgPixmapCache();

    bool load(const QString& url);
    bool isValid() const;

    void render(QPainter* painter, const QRectF& bounds);
    QSize defaultSize() const;

private:
    Q_DISABLE_COPY(QtSvgPixmapCache)
    QtSvgPixmapCachePrivate * const d;
};

#endif // QT_SVG_PIXMAP_CACHE
