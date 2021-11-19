//
//
#include <QSurfaceFormat>
#include <QQuickWindow>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

void initDefaultGraphicsSettings(void)
{
    QSurfaceFormat fmt = QSurfaceFormat::defaultFormat();

    fmt.setVersion(3, 3);
    fmt.setProfile(QSurfaceFormat::CoreProfile);
    fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

#ifdef VOLCANO_DEBUG
    fmt.setOption(QSurfaceFormat::DebugContext, true);
#endif

    fmt.setRedBufferSize(8);
    fmt.setGreenBufferSize(8);
    fmt.setBlueBufferSize(8);
    fmt.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(fmt);

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
}

VOLCANO_GRAPHICS_END
