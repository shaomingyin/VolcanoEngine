#ifndef VOLCANO_WORLD_CONTEXT_H
#define VOLCANO_WORLD_CONTEXT_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Context {
public:
    virtual ~Context() = default;

public:
    virtual void requestExit() = 0;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONTEXT_H
