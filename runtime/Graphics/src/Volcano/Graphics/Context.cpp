//
//
#include <Volcano/Graphics/Context.h>

VOLCANO_GRAPHICS_BEGIN

namespace BuiltinShaders {
}

thread_local Context* Context::current_ = nullptr;

Context::Context() {
}

Context::~Context() {
}

void Context::makeCurrent() {
	current_ = this;
}

VOLCANO_GRAPHICS_END
