//
//
#ifndef VOLCANO_GRAPHICS_SCENE_HPP
#define VOLCANO_GRAPHICS_SCENE_HPP

#include <Volcano/Game/Scene.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Context.hpp>

VOLCANO_GRAPHICS_BEGIN

class Scene: public QObject {
    Q_OBJECT

public:
    Scene(Context &context, Game::Scene* gameScene, QObject *parent = nullptr);
    ~Scene(void) override;

public:
    Context &context(void);
    const Context &context(void) const;
    Game::Scene* gameScene();
    const Game::Scene* gameScene() const;

private slots:
    void onGameEntityAdded(Game::Entity* p);
    void onGameEntityRemoved(Game::Entity* p);

private:
    Context &context_;
    Game::Scene* gameScene_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SCENE_HPP
