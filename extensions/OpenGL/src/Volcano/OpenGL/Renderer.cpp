//
//
#include <Volcano/OpenGL/Renderer.h>

VOLCANO_OPENGL_BEGIN

Renderer::Renderer() {
}

Graphics::Buffer* Renderer::createBuffer(Graphics::Buffer::Type type, uint32_t size) {
	return nullptr;
}

Graphics::Shader* Renderer::createShader(Graphics::Shader::Type type) {
	return nullptr;
}

Graphics::Program* Renderer::createProgram() {
	return nullptr;
}

Graphics::Texture* Renderer::createTexture(Graphics::Texture::Type type, Graphics::Texture::Format format, Graphics::Texture::Usage usage, uint32_t width, uint32_t height, uint32_t depth) {
	return nullptr;
}

void Renderer::draw(const Graphics::View& view, Graphics::Target& target) noexcept {
}

VOLCANO_OPENGL_END
