//
//
#ifndef VOLCANO_GRAPHICS_MESH_HPP
#define VOLCANO_GRAPHICS_MESH_HPP

#include <QMetaObject>

#include <Volcano/Game/Mesh.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Mesh {
public:
    Mesh(Game::Mesh *gameMesh);
    Mesh(Mesh &&that);
    Mesh(const Mesh &that);
    virtual ~Mesh(void);

public:
    Mesh &operator=(Mesh &&that);
    Mesh &operator=(const Mesh &that);

private:
    void init(void);
    void onGameMeshSourceChanged(const QUrl &url);

private:
    Game::Mesh *m_gameMesh;
    QMetaObject::Connection m_onGameMeshSourceChangedConnection;
    QIODevice *m_vertexBuffer;
    QIODevice *m_vertexIndexBuffer;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RESOURCE_HPP
