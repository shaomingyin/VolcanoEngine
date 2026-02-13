//
//
#ifndef VOLCANO_LAUNCHER_CLIENT_H
#define VOLCANO_LAUNCHER_CLIENT_H

#include <string>

#include <Volcano/Launcher/Common.h>
#include <Volcano/Launcher/Local.h>

VOLCANO_LAUNCHER_BEGIN

class Client: public Local {
public:
    Client(rttr::type world_type);
    virtual ~Client() = default;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_CLIENT_H
