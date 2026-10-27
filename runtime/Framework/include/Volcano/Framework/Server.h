#ifndef VOLCANO_FRAMEWORK_SERVER_H
#define VOLCANO_FRAMEWORK_SERVER_H

#include <Volcano/Framework/Base.h>

VOLCANO_FRAMEWORK_BEGIN

class Server: public Base {
public:
    Server();
    ~Server() override;

protected:
    void onBeforeUpdate(Clock::duration elapsed) override;
    virtual void onNetworkUpdate(Clock::duration elapsed);
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_SERVER_H
