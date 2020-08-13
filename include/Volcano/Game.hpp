//
//
#ifndef VOLCANO_GAME_HPP
#define VOLCANO_GAME_HPP

#include <QQuickItem>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API Game: public QQuickItem
{
    Q_OBJECT

public:
    Game(QQuickItem *parent = nullptr);
    ~Game(void) override;
};

VOLCANO_END

#endif // VOLCANO_GAME_HPP
