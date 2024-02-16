//
//
#ifndef VOLCANO_SCREEN_HPP
#define VOLCANO_SCREEN_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Actor.hpp>

VOLCANO_BEGIN

class Screen: public Actor {
    Q_OBJECT

public:
    Screen(QObject* parent = nullptr);
    ~Screen() override = default;
};

VOLCANO_END

#endif // VOLCANO_SCREEN_HPP
