//
//
#ifndef VOLCANO_GAME_MODEL_HPP
#define VOLCANO_GAME_MODEL_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_API Model: public Component
{
    Q_OBJECT

public:
    Model(QObject *parent = nullptr);
    ~Model(void) override;

public:
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_MODEL_HPP
