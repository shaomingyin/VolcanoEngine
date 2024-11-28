//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Pipeline.h>

VOLCANO_GRAPHICS_BEGIN

Pipeline::Pipeline() {
	enable(BoolAttribute::Clear);
	set(Vector4fAttribute::ClearColor, { 0.0f, 0.0f, 0.0f, 0.0f });
}

Pipeline::~Pipeline() {
}

void Pipeline::reset() {
}

VOLCANO_GRAPHICS_END
