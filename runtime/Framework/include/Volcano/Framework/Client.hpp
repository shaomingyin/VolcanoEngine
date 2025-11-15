//
//
#ifndef VOLCANO_FRAMEWORK_CLIENT_HPP
#define VOLCANO_FRAMEWORK_CLIENT_HPP

#include <string>

#include <Volcano/Framework/Common.hpp>
#include <Volcano/Framework/Local.hpp>

VOLCANO_FRAMEWORK_BEGIN

class Client: public Local {
public:
    Client(WorldCreator world_creator);
    virtual ~Client() = default;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CLIENT_HPP
