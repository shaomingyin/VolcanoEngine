//
//
#ifndef VOLCANO_GRAPHICS_COMMON_H
#define VOLCANO_GRAPHICS_COMMON_H

#include <QThread>
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_5_Core>

#include <Volcano/Common.h>
#include <Volcano/Graphics/Config.h>

#define VOLCANO_GRAPHICS_BEGIN VOLCANO_BEGIN namespace Graphics {
#define VOLCANO_GRAPHICS_END } VOLCANO_END

VOLCANO_GRAPHICS_BEGIN

class OpenGLFunctions final {
public:
    using Functions = QOpenGLFunctions_4_5_Core;

public:
    OpenGLFunctions();

public:
    Functions* operator()(QOpenGLContext* context = nullptr) {
        if (Q_LIKELY(functions_ != nullptr)) {
            return functions_;
        }
        init(context);
        Q_ASSERT(functions_ != nullptr);
        return functions_;
    }

private:
    void init(QOpenGLContext* context);

private:
    QOpenGLContext* context_;
    Functions* functions_;
};

extern thread_local OpenGLFunctions gl;

inline GLenum bufferBinding(GLenum type) {
    GLenum pn;

    switch (type) {
    case GL_ARRAY_BUFFER:
        pn = GL_ARRAY_BUFFER_BINDING;
        break;
    case GL_ELEMENT_ARRAY_BUFFER:
        pn = GL_ELEMENT_ARRAY_BUFFER_BINDING;
        break;
    case GL_PIXEL_PACK_BUFFER:
        pn = GL_PIXEL_PACK_BUFFER_BINDING;
        break;
    case GL_PIXEL_UNPACK_BUFFER:
        pn = GL_PIXEL_UNPACK_BUFFER_BINDING;
        break;
    default:
        pn = GL_INVALID_ENUM;
        break;
    }

    return pn;
}

inline GLint currentBuffer(GLenum type) {
    GLenum binding = bufferBinding(type);
    Q_ASSERT(binding != GL_INVALID_ENUM);
    GLint id;
    gl()->glGetIntegerv(binding, &id);
    return id;
}

inline GLint currentProgram() {
    GLint id;
    gl()->glGetIntegerv(GL_CURRENT_PROGRAM, &id);
    return id;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_COMMON_H
