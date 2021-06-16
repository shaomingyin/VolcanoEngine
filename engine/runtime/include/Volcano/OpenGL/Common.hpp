//
//
#ifndef VOLCANO_OPENGL_COMMON_HPP
#define VOLCANO_OPENGL_COMMON_HPP

#include <QVector3D>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>

#include <Volcano/Common.hpp>

#define VOLCANO_OPENGL_BEGIN VOLCANO_BEGIN namespace OpenGL {
#define VOLCANO_OPENGL_END } VOLCANO_END

VOLCANO_OPENGL_BEGIN

struct Vertex {
    QVector3D pos;
    QVector3D normal;
    QVector2D texCoord;
};

using VertexIndex = quint32;

using Functions = QOpenGLFunctions_3_3_Core;

VOLCANO_INLINE Functions *currentFunctions(void)
{
    auto p = QOpenGLVersionFunctionsFactory::get<Functions>();
    Q_ASSERT(p != nullptr);
    return p;
}

void makeSurfaceFormat(QSurfaceFormat &out);

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_COMMON_HPP
