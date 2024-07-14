//
//
#ifndef VOLCANO_GAME_SCREEN_HPP
#define VOLCANO_GAME_SCREEN_HPP

#include <QQuickRenderControl>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Screen: public Actor {
    Q_OBJECT

public:
    Screen(QObject* parent = nullptr);
    ~Screen() override = default;

private:
    QQuickRenderControl render_control_;
    QQuickWindow* render_window_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCREEN_HPP
