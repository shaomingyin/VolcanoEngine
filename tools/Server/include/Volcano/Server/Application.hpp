//
//
#ifndef VOLCANO_SERVER_APPLICATION_HPP
#define VOLCANO_SERVER_APPLICATION_HPP

#include <string>

#include <Volcano/World/Base.hpp>
#include <Volcano/Server/Common.hpp>

VOLCANO_SERVER_BEGIN

class Server: public World::Base {
public:
    Server(WorldCreator world_creator);
    virtual ~Server() = default;
};

VOLCANO_SERVER_END

#endif // VOLCANO_SERVER_APPLICATION_HPP
