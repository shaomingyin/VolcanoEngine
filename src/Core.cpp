//
//
#include <QQmlEngine>

#include <Volcano.h>

VOLCANO_BEGIN

VOLCANO_API bool init(void)
{
    registerType<Entity>("Volcano", "Entity");
    registerType<Scene>("Volcano", "Scene");

    registerType<Renderer::Mesh>("Volcano.Renderer", "Mesh");
    registerType<Renderer::Light>("Volcano.Renderer", "Light");
    registerType<Renderer::Camera>("Volcano.Renderer", "Camera");

    return true;
}

VOLCANO_END
