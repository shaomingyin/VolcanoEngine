//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_BUFFER_HPP
#define VOLCANO_GRAPHICS_OPENGL_BUFFER_HPP

#include <QIODevice>

#include <Volcano/Graphics/OpenGL/Common.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class Buffer: public QIODevice {
    Q_OBJECT

public:
    Buffer(QObject *parent = nullptr);
    ~Buffer(void) override;

public:
    virtual void bind(void) = 0;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_BUFFER_HPP
