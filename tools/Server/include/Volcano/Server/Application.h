//
//
#ifndef VOLCANO_SERVER_APPLICATION_H
#define VOLCANO_SERVER_APPLICATION_H

#include <string>

#include <Volcano/World/Base.h>
#include <Volcano/Server/Common.h>

VOLCANO_SERVER_BEGIN

class Server: public World::Base {
public:
    Server(WorldCreator world_creator);
    virtual ~Server() = default;
};

VOLCANO_SERVER_END

#endif // VOLCANO_SERVER_APPLICATION_H
