//
//
#include <QQuickWindow>
#include <QSurfaceFormat>

#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/CameraView.hpp>
#include <Volcano/Graphics/Init.hpp>

VOLCANO_GRAPHICS_BEGIN

bool init(void)
{
    registerUncreatableType<Camera>("volcano.graphics", "Camera");

    registerType<CameraView>("volcano.graphics", "CameraView");

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    QSurfaceFormat format;

    format.setVersion(3, 3);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setDepthBufferSize(24);

#ifdef VOLCANO_DEBUG
    format.setOption(QSurfaceFormat::DebugContext, true);
#endif

    QSurfaceFormat::setDefaultFormat(format);

    return true;
}

VOLCANO_GRAPHICS_END
