//
//
#ifndef VOLCANO_SYSTEM_SERVER_HPP
#define VOLCANO_SYSTEM_SERVER_HPP

#include <Volcano/Game/World.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/Engine.hpp>

VOLCANO_SYSTEM_BEGIN

class Server: public Engine {
    Q_OBJECT

public:
    Server(QObject* parent = nullptr);

private:
    Game::World gameWorld_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVER_HPP
