//
//
#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

Functions *currentFunctions(void)
{
    return QOpenGLVersionFunctionsFactory::get<Functions>();
}

void makeDefaultSurfaceFormat(void)
{
    QSurfaceFormat format;

    format = QSurfaceFormat::defaultFormat();

    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

#ifdef VOLCANO_DEBUG
    format.setOption(QSurfaceFormat::DebugContext, true);
#endif

    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setDepthBufferSize(24);

    QSurfaceFormat::setDefaultFormat(format);
}

VOLCANO_OPENGL_END
