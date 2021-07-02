//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>

#include <Volcano/Common.hpp>

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

using OpenGLFunctions = QOpenGLFunctions_3_3_Core;

VOLCANO_INLINE OpenGLFunctions *glFunctions(void)
{
    return QOpenGLVersionFunctionsFactory::get<OpenGLFunctions>();
}

struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
};

using VertexIndex = quint32;

QIODevice *allocStaticVertexBuffer(int count);
QIODevice *allocStaticVertexIndexBuffer(int count);
QIODevice *allocDynamicVertexBuffer(int count);
QIODevice *allocDynamicVertexIndexBuffer(int count);
QIODevice *allocTexture(int width, int height, GLenum format);

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_HPP
