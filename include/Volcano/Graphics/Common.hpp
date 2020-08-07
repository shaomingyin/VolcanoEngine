//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <QVector3D>
#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Common.hpp>

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

typedef QOpenGLFunctions_3_3_Core OpenGLFunctions;

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector3D texCoord;
};

typedef quint32 VertexIndex;

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_HPP
