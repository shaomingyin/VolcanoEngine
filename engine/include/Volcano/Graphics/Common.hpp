//
//
#ifndef VOLCANO_GRAPHICS_COMMON_HPP
#define VOLCANO_GRAPHICS_COMMON_HPP

#include <QIODevice>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_3_Core>

#include <Volcano/Common.hpp>

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

using OpenGLFunctions = QOpenGLFunctions_3_3_Core;

struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
};

using VertexIndex = quint32;

class Buffer: public QIODevice {
    Q_OBJECT

public:
    Buffer(QObject *parent = nullptr);
    ~Buffer(void) override;

public:
    virtual void bind(void) = 0;
};

OpenGLFunctions *glFunctions(void);
Buffer *allocStaticVertexBuffer(int count);
Buffer *allocStaticVertexIndexBuffer(int count);
Buffer *allocDynamicVertexBuffer(int count);
Buffer *allocDynamicVertexIndexBuffer(int count);
// Texture *allocTexture(int width, int height, GLenum format);
// QOpenGLShaderProgram *builtinShaderProgram(type ...)

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_HPP
