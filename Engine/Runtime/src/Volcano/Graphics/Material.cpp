//
//
#include <Volcano/Graphics/Material.hpp>

VOLCANO_GRAPHICS_BEGIN

Material::Material(Game::Material *p, QObject *parent):
    QObject(parent),
    m_gameMaterial(p)
{
    Q_ASSERT(p != nullptr);
    connect(p, &Game::Material::sourceChanged, this, &Material::onSourceChanged);
    onSourceChanged(p->source());
}

Material::~Material(void)
{
}

Game::Material *Material::gameMaterial(void)
{
    return m_gameMaterial;
}

void Material::onSourceChanged(const QUrl &v)
{
    // TODO post a init task to renderer thread.
}

VOLCANO_GRAPHICS_END
