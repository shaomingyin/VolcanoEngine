//
//
#include <memory>

#include <Volcano/Game/Graphics/Mesh.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

Mesh::Mesh(QObject *parent):
    Resource(parent),
    m_glMesh(nullptr),
    m_needReloading(false)
{
}

Mesh::~Mesh(void)
{
}

OpenGL::Mesh *Mesh::glMesh(void)
{
    Q_ASSERT(QOpenGLContext::currentContext() != nullptr);

    if (m_glMesh != nullptr) {
        if (!m_needReloading)
            return m_glMesh;
        delete m_glMesh;
        m_glMesh = nullptr;
        m_needReloading = false;
    }

    // TODO start loading...

    return nullptr;
}

void Mesh::onSourceChanged(const QUrl &v)
{
    m_needReloading = true;
}

VOLCANO_GAME_GRAPHICS_END
