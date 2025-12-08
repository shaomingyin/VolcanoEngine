//
//
#ifndef VOLCANO_GAME_MODEL_H
#define VOLCANO_GAME_MODEL_H

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Loadable.h>

VOLCANO_GAME_BEGIN

class Model: public Loadable {
    Q_OBJECT

public:
    Model(QObject* parent = nullptr);

public:
    friend QDataStream& operator<<(QDataStream& s, const Model& v);
    friend QDataStream& operator>>(QDataStream& s, Model& v);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MODEL_H
