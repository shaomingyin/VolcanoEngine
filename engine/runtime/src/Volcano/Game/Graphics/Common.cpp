//
//
#include <Volcano/Game/Graphics/Common.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

OpenGLFunctions *glFunctions(QOpenGLContext *context)
{
    return QOpenGLVersionFunctionsFactory::get<OpenGLFunctions>(context);
}

void glMakeDefaultSurfaceFormat(QSurfaceFormat &out)
{
    out = QSurfaceFormat::defaultFormat();

    out.setVersion(3, 3);
    out.setProfile(QSurfaceFormat::CoreProfile);

#ifdef VOLCANO_DEBUG
    out.setOption(QSurfaceFormat::DebugContext, true);
#endif

    out.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    out.setDepthBufferSize(24);
}

VOLCANO_GAME_GRAPHICS_END
