//
//
#ifndef VOLCANO_WORLD_MODEL_H
#define VOLCANO_WORLD_MODEL_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Loadable.h>

VOLCANO_WORLD_BEGIN

class Model: public Loadable {
    Q_OBJECT

public:
    Model(QObject* parent = nullptr);

public:
    friend QDataStream& operator<<(QDataStream& s, const Model& v);
    friend QDataStream& operator>>(QDataStream& s, Model& v);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_MODEL_H
