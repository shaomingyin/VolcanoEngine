//
//
#ifndef VOLCANO_UI_GRAPHICS_BUFFER_HPP
#define VOLCANO_UI_GRAPHICS_BUFFER_HPP

#include <QIODevice>

#include <Volcano/UI/Graphics/Common.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API Buffer: public QIODevice {
    Q_OBJECT

public:
    Buffer(QObject *parent = nullptr);
    ~Buffer(void) override;

public:
    virtual void bind(void) = 0;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRAPHICS_BUFFER_HPP
