//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <QVector3D>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>

#include <Volcano/Common.hpp>
#include <Volcano/Graphics/Config.hpp>

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

struct Vertex {
    QVector3D pos;
    QVector3D normal;
    QVector2D texCoord;
};

using VertexIndex = quint32;

using OpenGLFunctions = QOpenGLFunctions_3_3_Core;

VOLCANO_INLINE OpenGLFunctions *gl(void)
{
    auto glFunctions = QOpenGLVersionFunctionsFactory::get<OpenGLFunctions>();
    Q_ASSERT(glFunctions != nullptr);
    return glFunctions;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_HPP
