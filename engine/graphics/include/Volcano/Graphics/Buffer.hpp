//
//
#ifndef VOLCANO_GRAPHICS_BUFFER_HPP
#define VOLCANO_GRAPHICS_BUFFER_HPP

#include <QIODevice>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_GRAPHICS_API Buffer: public QIODevice {
    Q_OBJECT

public:
    Buffer(QObject *parent = nullptr);
    ~Buffer(void) override;

public:
    virtual void bind(void) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFER_HPP
