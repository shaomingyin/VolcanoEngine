//
//
#ifndef VOLCANO_GAME_GRAPHICS_COMMON_HPP
#define VOLCANO_GAME_GRAPHICS_COMMON_HPP

#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>

#include <Volcano/Game/Common.hpp>

#define VOLCANO_GAME_GRAPHICS_BEGIN VOLCANO_GAME_BEGIN namespace Graphics {
#define VOLCANO_GAME_GRAPHICS_END } VOLCANO_GAME_END

VOLCANO_GAME_GRAPHICS_BEGIN

struct Vertex {
    GLfloat position[3];
    GLfloat color[4];
    GLfloat normal[3];
    GLfloat texCoord[2];
};

using VertexIndex = quint32;

using OpenGLFunctions = QOpenGLFunctions_3_3_Core;

OpenGLFunctions *glFunctions(QOpenGLContext *context = nullptr);
void glMakeDefaultSurfaceFormat(QSurfaceFormat &out);

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_COMMON_HPP
