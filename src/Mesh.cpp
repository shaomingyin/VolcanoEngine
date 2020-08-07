//
//
#include <Volcano/Mesh.hpp>

VOLCANO_BEGIN

Mesh::Mesh(Node *parent):
    Resource(parent),
    m_vertexBuffer(nullptr),
    m_vertexIndexBuffer(nullptr)
{
    connect(this, &Resource::sourceChanged, this, &Mesh::onSourceChanged);
}

Mesh::~Mesh(void)
{
    release();
}

bool Mesh::init(Graphics::Memory &gm)
{
    return true;
}

void Mesh::onSourceChanged(const QUrl &url)
{
    release();


}

void Mesh::release(void)
{
    if (m_vertexBuffer != nullptr)
        delete m_vertexBuffer;
    if (m_vertexIndexBuffer != nullptr)
        delete m_vertexIndexBuffer;
}

VOLCANO_END
