//
//
#ifndef VOLCANO_FRAMEWORK_SERVER_H
#define VOLCANO_FRAMEWORK_SERVER_H

#include <string>

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Base.h>

VOLCANO_FRAMEWORK_BEGIN

class Server: public Base {
public:
    Server(const std::string& name);
    virtual ~Server() = default;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_SERVER_H
