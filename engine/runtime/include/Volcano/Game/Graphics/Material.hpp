//
//
#ifndef VOLCANO_GAME_GRAPHICS_MATERIAL_HPP
#define VOLCANO_GAME_GRAPHICS_MATERIAL_HPP

#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Graphics/Common.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

class Material: public Component {
public:
    Material(QObject *parent = nullptr);
    virtual ~Material(void);
};

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_MATERIAL_HPP
