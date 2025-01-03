//
//
#ifndef VOLCANO_WORLD_MESH_H
#define VOLCANO_WORLD_MESH_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Resource.h>

VOLCANO_WORLD_BEGIN

class Mesh: public Resource {
    Q_OBJECT

public:
    Mesh(QObject* parent = nullptr);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MESH_H
