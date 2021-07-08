//
//
#include <QObject>

#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

Mesh::Mesh(Game::Mesh *gameMesh):
    m_gameMesh(gameMesh),
    m_vertexBuffer(nullptr),
    m_vertexIndexBuffer(nullptr)
{
    init();
}

Mesh::Mesh(Mesh &&that):
    m_gameMesh(that.m_gameMesh),
    m_vertexBuffer(that.m_vertexBuffer),
    m_vertexIndexBuffer(that.m_vertexIndexBuffer)
{
    that.m_gameMesh = nullptr;
    that.m_vertexBuffer = nullptr;
    that.m_vertexIndexBuffer = nullptr;

    if (that.m_onGameMeshSourceChangedConnection)
        QObject::disconnect(that.m_onGameMeshSourceChangedConnection);
}

Mesh::Mesh(const Mesh &that):
    m_gameMesh(that.m_gameMesh)
{
    // TODO
}

Mesh::~Mesh(void)
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;
    if (m_vertexIndexBuffer != nullptr)
        delete m_vertexIndexBuffer;
}

void Mesh::init(void)
{
    Q_ASSERT(m_gameMesh != nullptr);
    Q_ASSERT(m_vertexBuffer == nullptr);
    Q_ASSERT(m_vertexIndexBuffer == nullptr);

    auto source = m_gameMesh->source();
    if (source.isValid())
        onGameMeshSourceChanged(source);

    m_onGameMeshSourceChangedConnection =
        QObject::connect(m_gameMesh, &Game::Mesh::sourceChanged, [this] (const QUrl &url) {
            onGameMeshSourceChanged(url);
        });
}

void Mesh::onGameMeshSourceChanged(const QUrl &url)
{

}

Mesh &Mesh::operator=(Mesh &&that)
{
    m_gameMesh = that.m_gameMesh;
    that.m_gameMesh = nullptr;

    m_vertexBuffer = that.m_vertexBuffer;
    that.m_vertexBuffer = nullptr;

    m_vertexIndexBuffer = that.m_vertexIndexBuffer;
    that.m_vertexIndexBuffer = nullptr;

    QObject::disconnect(that.m_onGameMeshSourceChangedConnection);

    return (*this);
}

Mesh &Mesh::operator=(const Mesh &that)
{
    m_gameMesh = that.m_gameMesh;

    // TODO

    return (*this);
}

VOLCANO_GRAPHICS_END
