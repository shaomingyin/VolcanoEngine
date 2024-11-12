//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/Context.h>

VOLCANO_GRAPHICS_BEGIN

namespace BuiltinShaders {
}

thread_local Context* Context::current_ = nullptr;

Context::Context(GL3WGetProcAddressProc get_proc) {
	if (get_proc == nullptr) {
		throw Error(Errc::InvalidParameter);
	}
	int ret = gl3wInit(&gl3w_, get_proc);
	if (ret != GL3W_OK) {
		throw Error(Errc::InvalidParameter);
	}
}

Context::~Context() {
}

void Context::makeCurrent() {
	gl3wProcs = &gl3w_;

	// TODO

	current_ = this;
}

VOLCANO_GRAPHICS_END
