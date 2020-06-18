//
//
#include <Volcano/Renderer.h>

VOLCANO_BEGIN

Renderer::Renderer(Scene *scene):
    Scene::Listener(scene)
{
}

Renderer::~Renderer(void)
{
}

void Renderer::update(float elapsed)
{
}

void Renderer::render(void)
{
}

VOLCANO_END
