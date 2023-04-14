//
//
#ifndef VOLCANO_GRAPHICS_CONTEXT_HPP
#define VOLCANO_GRAPHICS_CONTEXT_HPP

#include <QObject>

#include <Volcano/Game/World.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(Game::World &gameWorld, QObject *parent = nullptr);
    ~Context(void) override;

public:
    virtual bool Init(void);
    virtual void Update(Duration elapsed);
    const Game::World &gameWorld(void) const;

private slots:
    void onGameWorldSceneAdded(Game::Scene *p);
    void onGameWorldSceneRemoved(Game::Scene *p);

private:
    Game::World &m_gameWorld;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CONTEXT_HPP
