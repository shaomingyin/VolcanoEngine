//
//
#include <Volcano/Graphics/Scene.hpp>

VOLCANO_GRAPHICS_BEGIN

Scene::Scene(Context &context, QObject *parent)
    : QObject(parent)
    , m_context(context)
{
}

Scene::~Scene(void)
{
}

Context &Scene::context(void)
{
    return m_context;
}

const Context &Scene::context(void) const
{
    return m_context;
}

VOLCANO_GRAPHICS_END
