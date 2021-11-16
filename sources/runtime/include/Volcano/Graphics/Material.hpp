//
//
#ifndef VOLCANO_GRAPHICS_MATERIAL_HPP
#define VOLCANO_GRAPHICS_MATERIAL_HPP

#include <Volcano/Game/Component.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Material: public Game::Component {
public:
    Material(QObject *parent = nullptr);
    virtual ~Material(void);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MATERIAL_HPP
