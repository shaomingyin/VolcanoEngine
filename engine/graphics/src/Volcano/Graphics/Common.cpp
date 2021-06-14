//
//
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

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

VOLCANO_GRAPHICS_END
