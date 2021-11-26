//
//
#ifndef VOLCANO_GAME_MESH_HPP
#define VOLCANO_GAME_MESH_HPP

#include <QFuture>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Mesh: public Component {
    Q_OBJECT

public:
    Mesh(QObject *parent = nullptr);
    ~Mesh(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MESH_HPP
