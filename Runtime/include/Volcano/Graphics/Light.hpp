//
//
#ifndef VOLCANO_GRAPHICS_LIGHT_HPP
#define VOLCANO_GRAPHICS_LIGHT_HPP

#include <QColor>

#include <Volcano/Graphics/Common.hpp>

#ifdef VOLCANO_GRAPHICS

VOLCANO_GRAPHICS_BEGIN

class Light {
public:
    Light(void);
    virtual ~Light(void);

public:
    const QColor &color(void) const;
    void setColor(const QColor &v);

private:
    QColor m_color;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS

#endif // VOLCANO_GRAPHICS_LIGHT_HPP
