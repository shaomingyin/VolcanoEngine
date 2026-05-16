//
//
#ifndef VOLCANO_WORLD_CONTEXT_H
#define VOLCANO_WORLD_CONTEXT_H

#include <async++.h>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Context {
public:
	Context() = default;
	virtual ~Context() = default;

public:
	virtual void schedule(async::task_run_handle task) = 0;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONTEXT_H
