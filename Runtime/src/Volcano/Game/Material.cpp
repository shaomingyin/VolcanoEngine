//
//
#include <Volcano/Game/Material.hpp>

VOLCANO_GAME_BEGIN

Material::Material(QObject *parent):
    Object(parent),
    m_graphicsMaterial(nullptr)
{
}

Material::~Material(void)
{
    if (m_graphicsMaterial != nullptr)
        delete m_graphicsMaterial;
}

void Material::onDraw(void)
{
    if (m_graphicsMaterial != nullptr)
        graphicsService()->bindMaterial(m_graphicsMaterial);
}

VOLCANO_GAME_END
