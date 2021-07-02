//
//
#ifndef VOLCANO_GRAPHICS_MESH_HPP
#define VOLCANO_GRAPHICS_MESH_HPP

#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Mesh {
public:
    Mesh(Game::Mesh *gameMesh);
    virtual ~Mesh(void);

public:
    bool init(void);
    const Game::Mesh *gameMesh(void) const;

private:
    Game::Mesh *m_gameMesh;
    QIODevice *m_vertexBuffer;
    QIODevice *m_vertexIndexBuffer;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RESOURCE_HPP
