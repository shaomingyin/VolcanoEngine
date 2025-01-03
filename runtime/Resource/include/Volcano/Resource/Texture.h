//
//
#ifndef VOLCANO_RESOURCE_TEXTURE_H
#define VOLCANO_RESOURCE_TEXTURE_H

#include <QPixmap>

#include <Volcano/Resource/Common.h>

VOLCANO_RESOURCE_BEGIN

class Texture {
public:
    Texture() = default;

public:
    QPixmap& pixmap() {
        return pixmap_;
    }

    const QPixmap& pixmap() const {
        return pixmap_;
    }

public:
    QPixmap pixmap_;
};

QDataStream &operator<<(QDataStream &s, const Texture& v);
QDataStream &operator>>(QDataStream &s, Texture& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_TEXTURE_H
