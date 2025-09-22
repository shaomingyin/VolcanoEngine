//
//
#ifndef VOLCANO_RESOURCE_TEXTURE_H
#define VOLCANO_RESOURCE_TEXTURE_H

#include <QPixmap>

#include <Volcano/Resource/Common.h>

VOLCANO_RESOURCE_BEGIN

struct Texture {
    QPixmap pixmap;
};

QDataStream &operator<<(QDataStream &s, const Texture& v);
QDataStream &operator>>(QDataStream &s, Texture& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_TEXTURE_H
