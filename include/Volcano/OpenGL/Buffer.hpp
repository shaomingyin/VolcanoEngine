//
//
#ifndef VOLCANO_OPENGL_BUFFER_HPP
#define VOLCANO_OPENGL_BUFFER_HPP

#include <QIODevice>

#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

class VOLCANO_API Buffer: public QIODevice
{
    Q_OBJECT

public:
    Buffer(QObject *parent = nullptr);
    ~Buffer(void);
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_BUFFER_HPP
