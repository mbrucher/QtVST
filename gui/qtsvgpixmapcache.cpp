/*
 Embedded Widgets Demo
 Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).*
 Contact:  Qt Software Information (qt-info@nokia.com)**
 This file may be used under the terms of the Embedded Widgets Demo License
 Agreement.
*/

#include "qtsvgpixmapcache.h"

#include <QtGui/QPainter>
#include <QtSvg/QSvgRenderer>

#if defined(Q_WS_HILDON) && (QT_VERSION < 0x040500)
#  define QT_MAEMO_XRENDER_ALPHA_BUG
#  warning Using workaround for 'XRender and QPixmap with alpha' drawing bug
#endif

class QtSvgPixmapCachePrivate
{
public:
    void updatePixmapCache(const QSizeF& size);

public:
    QSvgRenderer svgRenderer;
#ifdef QT_MAEMO_XRENDER_ALPHA_BUG
    QImage pixmapCache;
#else
    QPixmap pixmapCache;
#endif
};

QtSvgPixmapCache::QtSvgPixmapCache(QObject* parent) : QObject(parent)
    , d(new QtSvgPixmapCachePrivate())
{
}

QtSvgPixmapCache::QtSvgPixmapCache(const QString& url)
    : d(new QtSvgPixmapCachePrivate())
{
    load(url);
}

QtSvgPixmapCache::~QtSvgPixmapCache()
{
    delete d;
}

bool QtSvgPixmapCache::load(const QString& url)
{
    d->svgRenderer.load(url);
#ifdef QT_MAEMO_XRENDER_ALPHA_BUG
    d->pixmapCache = QImage();
#else
    d->pixmapCache = QPixmap();
#endif
    return d->pixmapCache.isNull();
}

bool QtSvgPixmapCache::isValid() const
{
    return d->svgRenderer.isValid();
}

void QtSvgPixmapCache::render(QPainter* painter, const QRectF& bounds)
{
    if (!d->svgRenderer.isValid()) {
        return;
    }

    if (d->pixmapCache.isNull() || bounds.size().toSize() != d->pixmapCache.size()) {
        d->updatePixmapCache(bounds.size());
    }

#ifdef QT_MAEMO_XRENDER_ALPHA_BUG
    painter->drawImage(bounds.topLeft(), d->pixmapCache);
#else
    painter->drawPixmap(bounds.topLeft(), d->pixmapCache);
#endif
}

QSize QtSvgPixmapCache::defaultSize() const
{
    return d->svgRenderer.defaultSize();
}

void QtSvgPixmapCachePrivate::updatePixmapCache(const QSizeF& size)
{
#ifdef QT_MAEMO_XRENDER_ALPHA_BUG
    pixmapCache = QImage(size.toSize(), QImage::Format_ARGB32_Premultiplied);
#else
    pixmapCache = QPixmap(size.toSize());
#endif
    pixmapCache.fill(Qt::transparent);

    QPainter painter(&pixmapCache);
    svgRenderer.render(&painter);
    painter.end();
}
