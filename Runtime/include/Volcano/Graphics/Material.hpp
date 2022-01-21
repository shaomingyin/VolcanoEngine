//
//
#ifndef VOLCANO_GRAPHICS_MATERIAL_HPP
#define VOLCANO_GRAPHICS_MATERIAL_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>

#ifdef VOLCANO_GRAPHICS

VOLCANO_GRAPHICS_BEGIN

class Material {
public:
    Material(void);
    virtual ~Material(void);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS

#endif // VOLCANO_GRAPHICS_MATERIAL_HPP
