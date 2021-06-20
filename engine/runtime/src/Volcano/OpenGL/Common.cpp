//
//
#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

void makeSurfaceFormat(QSurfaceFormat &out)
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

VOLCANO_OPENGL_END
