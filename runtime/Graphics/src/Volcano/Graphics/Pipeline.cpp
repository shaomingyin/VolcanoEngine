//
//
#include <Volcano/Error.h>
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

void Pipeline::apply() const {
	const auto& viewport = get(Vector4iAttribute::Viewport);
	glViewport(viewport.x(), viewport.y(), viewport.z(), viewport.w());

	const auto& clear_color = get(Vector4fAttribute::ClearColor);
	glClearColor(clear_color.x(), clear_color.y(), clear_color.z(), clear_color.w());

	if (bool_attributes_.test(static_cast<int>(BoolAttribute::Clear))) {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glUseProgram(program_.id());

	// TODO
}

VOLCANO_GRAPHICS_END
