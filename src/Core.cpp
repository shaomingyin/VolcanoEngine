//
//
#include <QSurfaceFormat>

#include <Volcano.h>

VOLCANO_BEGIN

VOLCANO_API bool init(void)
{
    QSurfaceFormat format;
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setDepthBufferSize(4);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(format);

    registerType<Node>("Volcano", "Node");
    registerType<Entity>("Volcano", "Entity");
    registerType<Scene>("Volcano", "Scene");
    registerType<Camera>("Volcano", "Camera");
    registerType<Viewport>("Volcano", "Viewport");

    registerType<Renderer::Light>("Volcano.Renderer", "Light");
    registerType<Renderer::Mesh>("Volcano.Renderer", "Mesh");

    return true;
}

VOLCANO_END
