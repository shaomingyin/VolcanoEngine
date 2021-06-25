//
//
#ifndef VOLCANO_OPENGL_COMMON_HPP
#define VOLCANO_OPENGL_COMMON_HPP

#include <QIODevice>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>

#include <Volcano/Common.hpp>

#define VOLCANO_OPENGL_BEGIN VOLCANO_BEGIN namespace OpenGL {
#define VOLCANO_OPENGL_END } VOLCANO_END

VOLCANO_OPENGL_BEGIN

struct Vertex {
    GLfloat position[3];
    GLfloat color[4];
    GLfloat normal[3];
    GLfloat texCoord[2];
};

using VertexIndex = quint32;

using Functions = QOpenGLFunctions_3_3_Core;

Functions *currentFunctions(void);
void makeDefaultSurfaceFormat(void);
QIODevice *allocStaticVertices(int count);
QIODevice *allocStaticVertexIndices(int count);
QIODevice *allocDynamicVertices(int count);
QIODevice *allocDynamicVertexIndices(int count);

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_COMMON_HPP
