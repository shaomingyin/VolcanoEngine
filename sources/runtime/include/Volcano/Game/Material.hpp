//
//
#ifndef VOLCANO_GAME_MATERIAL_HPP
#define VOLCANO_GAME_MATERIAL_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Material: public Component {
public:
    Material(QObject *parent = nullptr);
    virtual ~Material(void);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MATERIAL_HPP
