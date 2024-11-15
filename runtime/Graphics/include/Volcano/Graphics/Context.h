//
//
#ifndef VOLCANO_GRAPHICS_CONTEXT_H
#define VOLCANO_GRAPHICS_CONTEXT_H

#include <array>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Context {
public:
	Context(GL3WGetProcAddressProc get_proc);
	virtual ~Context();

public:
	void makeCurrent();

	static Context* current() {
		return current_;
	}

private:
	GL3WProcs gl3w_;
	static thread_local Context* current_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CONTEXT_H
