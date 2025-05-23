//
//
#ifndef VOLCANO_OPENGL_RENDERER_H
#define VOLCANO_OPENGL_RENDERER_H

#include <Volcano/Graphics/Renderer.h>
#include <Volcano/OpenGL/Common.h>

VOLCANO_OPENGL_BEGIN

class Renderer: public Graphics::Renderer {
	RTTR_ENABLE(Graphics::Renderer)

public:
	Renderer(World::Scene& scene);
	~Renderer() override;

public:
	Graphics::Window* createWindow(const std::string& name, int width, int height) override;
	Graphics::Texture* createTexture(int width, int height) override;
	void render(const Graphics::View& view, Duration elapsed, Graphics::Target* target) override;

private:
	
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_RENDERER_H
