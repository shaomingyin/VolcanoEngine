//
//
#ifndef VOLCANO_GAME_DYNAMICSCENE_HPP
#define VOLCANO_GAME_DYNAMICSCENE_HPP

#include <Volcano/Physics/World.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API DynamicScene: public Scene {
    Q_OBJECT

public:
    DynamicScene(QObject* parent = nullptr);
    DynamicScene(Context& context, QObject* parent = nullptr);

public:

signals:

protected:
    void updateView() const override;
    void updateState(Duration elapsed) override;

private:
    Physics::World physics_world_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DYNAMICSCENE_HPP
