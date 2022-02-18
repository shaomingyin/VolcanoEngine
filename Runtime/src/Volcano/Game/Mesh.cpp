//
//
#include <Volcano/Game/Mesh.hpp>

VOLCANO_GAME_BEGIN

Mesh::Mesh(QObject *parent):
    Component(parent),
    m_graphicsMesh(nullptr),
    m_material(nullptr)
{
}

Mesh::Mesh(Context *context, QObject *parent):
    Component(context, parent),
    m_graphicsMesh(nullptr),
    m_material(nullptr)
{
}

Mesh::~Mesh(void)
{
    if (m_graphicsMesh != nullptr)
        delete m_graphicsMesh;
}

const QUrl &Mesh::source(void) const
{
    return m_source;
}

void Mesh::setSource(const QUrl &v)
{
    if (m_source == v)
        return;

    m_source = v;
    reload();

    emit sourceChanged(v);
}

Material *Mesh::material(void)
{
    return m_material;
}

void Mesh::setMaterial(Material *p)
{
    if (m_material != p) {
        m_material = p;
        emit materialChanged(p);
    }
}

void Mesh::draw(void)
{
    if (m_graphicsMesh != nullptr) {
        if (m_material != nullptr)
            m_material->updateGraphics();
        graphicsService()->addMesh(m_graphicsMesh);
    }
}

void Mesh::reload(void)
{
    if (m_graphicsMesh != nullptr) {
        delete m_graphicsMesh;
        m_graphicsMesh = nullptr;
    }

    auto gService = graphicsService();
    if (gService == nullptr)
        return;

    m_graphicsMesh = graphicsService()->createMesh();
    if (m_graphicsMesh != nullptr) {
        // TODO
    }
}

VOLCANO_GAME_END
