//
//
#ifndef VOLCANO_UI_GRAPHICS_COMMON_HPP
#define VOLCANO_UI_GRAPHICS_COMMON_HPP

#include <QVector2D>
#include <QVector3D>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLVersionFunctionsFactory>
#include <QOpenGLFunctions_4_0_Core>

#include <Volcano/UI/Common.hpp>

#define VOLCANO_UI_GRAPHICS_BEGIN VOLCANO_UI_BEGIN namespace Graphics {
#define VOLCANO_UI_GRAPHICS_END } VOLCANO_UI_END

VOLCANO_UI_GRAPHICS_BEGIN

struct Vertex {
    QVector3D pos;
    QVector3D normal;
    QVector2D texCoord;
};

using VertexIndex = quint32;

using OpenGLFunctions = QOpenGLFunctions_4_0_Core;

VOLCANO_INLINE OpenGLFunctions *gl(void)
{
    static thread_local OpenGLFunctions *glFunctions = nullptr;

    if (Q_LIKELY(glFunctions != nullptr))
        return glFunctions;

    auto glContext = QOpenGLContext::currentContext();
    if (glContext != nullptr) {
        glFunctions = QOpenGLVersionFunctionsFactory::get<OpenGLFunctions>(glContext);
        if (!glFunctions->initializeOpenGLFunctions())
            glFunctions = nullptr;
    }

    return glFunctions;
}

VOLCANO_INLINE void makeSurfaceFormat(QSurfaceFormat &output)
{
    output.setVersion(4, 5);
    output.setProfile(QSurfaceFormat::CoreProfile);
    output.setSwapBehavior(QSurfaceFormat::DoubleBuffer);

    output.setRedBufferSize(8);
    output.setGreenBufferSize(8);
    output.setBlueBufferSize(8);
    output.setAlphaBufferSize(8);
    output.setDepthBufferSize(24);

    output.setStereo(false);
    output.setSwapInterval(1);

#if VOLCANO_DEBUG
    output.setOption(QSurfaceFormat::DebugContext, true);
#endif
}

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRAPHICS_COMMON_HPP
