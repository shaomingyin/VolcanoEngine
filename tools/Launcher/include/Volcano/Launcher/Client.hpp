//
//
#ifndef VOLCANO_LAUNCHER_CLIENT_HPP
#define VOLCANO_LAUNCHER_CLIENT_HPP

#include <string>

#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/Local.hpp>

VOLCANO_LAUNCHER_BEGIN

class Client: public Local {
public:
    Client(WorldCreator world_creator);
    virtual ~Client() = default;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_CLIENT_HPP
