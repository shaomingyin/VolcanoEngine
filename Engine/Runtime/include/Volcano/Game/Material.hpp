//
//
#ifndef VOLCANO_GAME_MATERIAL_HPP
#define VOLCANO_GAME_MATERIAL_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/LoadableComponent.hpp>

VOLCANO_GAME_BEGIN

class Material: public LoadableComponent {
    Q_OBJECT

public:
    Material(QObject *parent = nullptr);
    ~Material(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MATERIAL_HPP
