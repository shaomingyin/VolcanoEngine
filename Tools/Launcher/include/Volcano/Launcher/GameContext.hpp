//
//
#ifndef VOLCANO_LAUNCHER_GAMECONTEXT_HPP
#define VOLCANO_LAUNCHER_GAMECONTEXT_HPP

#include <QAtomicInt>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class GameContext: public Game::Context {
    Q_OBJECT

public:
    GameContext(QObject* parent = nullptr);

public:
    ResourceManager& resourceManager() override;
    bool beginView() override;
    void endView() override;
    Graphics::View& view() override;

private:
    ResourceManager resource_manager_;
    Graphics::View views_[2];
    QAtomicInt view_drawing_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_GAMECONTEXT_HPP
