//
//
#ifndef VOLCANO_GAME_GRAPHICS_MESH_HPP
#define VOLCANO_GAME_GRAPHICS_MESH_HPP

#include <QFuture>

#include <Volcano/Game/Resource.hpp>
#include <Volcano/Game/Graphics/Common.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

class Mesh: public Resource {
    Q_OBJECT

public:
    Mesh(QObject *parent = nullptr);
    virtual ~Mesh(void);

private:
    void load(void) override;

private:
    QIODevice *m_vertexBuffer;
    QIODevice *m_vertexIndexBuffer;
};

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_MESH_HPP
