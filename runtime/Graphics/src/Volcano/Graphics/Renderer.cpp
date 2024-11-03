//
//
#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Renderer.h>

VOLCANO_GRAPHICS_BEGIN

Renderer::Renderer() {
	enable(BoolAttribute::Clear);
	set(Vector4fAttribute::ClearColor, { 0.0f, 0.0f, 0.0f, 0.0f });
}

Renderer::~Renderer() {
}

void Renderer::render(const View& view, Target& target) {
	if (!target.beginFrame()) {
		return;
	}

	auto frame_guard = scopeGuard([&] {
		target.endFrame();
	});

	glViewport(0, 0, target.width(), target.height());

	const auto& clear_color = vector4f_attributes_[static_cast<int>(Vector4fAttribute::ClearColor)];
	glClearColor(clear_color.x(), clear_color.y(), clear_color.z(), clear_color.w());

	if (bool_attributes_.test(static_cast<int>(BoolAttribute::Clear))) {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

VOLCANO_GRAPHICS_END
