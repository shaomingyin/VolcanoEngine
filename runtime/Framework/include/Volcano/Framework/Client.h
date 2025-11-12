//
//
#ifndef VOLCANO_FRAMEWORK_CLIENT_H
#define VOLCANO_FRAMEWORK_CLIENT_H

#include <string>

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Local.h>

VOLCANO_FRAMEWORK_BEGIN

class Client: public Local {
public:
    Client(WorldCreator world_creator);
    virtual ~Client() = default;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CLIENT_H
