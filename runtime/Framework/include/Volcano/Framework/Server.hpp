//
//
#ifndef VOLCANO_FRAMEWORK_SERVER_HPP
#define VOLCANO_FRAMEWORK_SERVER_HPP

#include <string>

#include <Volcano/Framework/Common.hpp>
#include <Volcano/Framework/Base.hpp>

VOLCANO_FRAMEWORK_BEGIN

class Server: public Base {
public:
    Server(WorldCreator world_creator);
    virtual ~Server() = default;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_SERVER_HPP
