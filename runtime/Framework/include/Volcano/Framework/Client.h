#ifndef VOLCANO_FRAMEWORK_CLIENT_H
#define VOLCANO_FRAMEWORK_CLIENT_H

#include <Volcano/Framework/Local.h>

VOLCANO_FRAMEWORK_BEGIN

class Client: public Local {
public:
    using Local::Local;
    ~Client() override;

protected:
    void onBeforeUpdate(Clock::duration elapsed) override;
    virtual void onNetworkUpdate(Clock::duration elapsed);
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CLIENT_H
