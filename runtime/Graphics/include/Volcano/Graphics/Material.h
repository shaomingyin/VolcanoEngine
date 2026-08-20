//
//
#ifndef VOLCANO_WORLD_MATERIAL_H
#define VOLCANO_WORLD_MATERIAL_H

#include <QObject>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Material: public QObject {
public:
    Material(QObject* parent = nullptr);

public:
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MATERIAL_H
