//
//
#ifndef VOLCANO_GRAPHICS_BUFFER_HPP
#define VOLCANO_GRAPHICS_BUFFER_HPP

#include <functional>

#include <QIODevice>
#include <QOpenGLBuffer>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Buffer: public QIODevice
{
    Q_OBJECT

public:
    Buffer(QObject *parent = nullptr);
    ~Buffer(void) override;

public:
    virtual int offset(void) const = 0;
    virtual void *data(int pos = 0) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFER_HPP
