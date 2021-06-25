//
//
#ifndef VOLCANO_GAME_GRAPHICS_MESH_HPP
#define VOLCANO_GAME_GRAPHICS_MESH_HPP

#include <QFuture>

#include <Volcano/OpenGL/Mesh.hpp>
#include <Volcano/Game/Resource.hpp>
#include <Volcano/Game/Graphics/Common.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

class Mesh: public Resource {
    Q_OBJECT

public:
    Mesh(QObject *parent = nullptr);
    virtual ~Mesh(void);

public:
    OpenGL::Mesh *glMesh(void);

private:
    void onSourceChanged(const QUrl &v) override;

private:
    OpenGL::Mesh *m_glMesh;
    bool m_needReloading;
};

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_MESH_HPP
