//
//
#ifndef VOLCANO_OPENGL_RENDERER_H
#define VOLCANO_OPENGL_RENDERER_H

#include <Volcano/Graphics/Renderer.h>
#include <Volcano/OpenGL/Common.h>

VOLCANO_OPENGL_BEGIN

class Renderer : public Graphics::Renderer {
public:
	Renderer();
	virtual ~Renderer() = default;

protected:
	Graphics::Buffer* createBuffer(Graphics::Buffer::Type type, uint32_t size) override;
	Graphics::Shader* createShader(Graphics::Shader::Type type) override;
	Graphics::Program* createProgram() override;
	Graphics::Texture* createTexture(Graphics::Texture::Type type, Graphics::Texture::Format format, Graphics::Texture::Usage usage, uint32_t width, uint32_t height, uint32_t depth) override;
	void draw(const Graphics::View& view, Graphics::Target& target) noexcept override;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_RENDERER_H
