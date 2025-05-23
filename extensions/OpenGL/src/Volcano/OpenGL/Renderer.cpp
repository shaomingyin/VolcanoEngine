//
//
#include <Volcano/OpenGL/Window.h>
#include <Volcano/OpenGL/Renderer.h>

VOLCANO_OPENGL_BEGIN

Renderer::Renderer(World::Scene& scene)
    : Graphics::Renderer(scene) {
}

Renderer::~Renderer() {
}

Graphics::Window* Renderer::createWindow(const std::string& title, int width, int height) {
    return new Window(title, width, height);
}

Graphics::Texture* Renderer::createTexture(int width, int height) {
    return nullptr;
}

void Renderer::render(const Graphics::View& view, Duration elapsed, Graphics::Target* target) {
}

VOLCANO_OPENGL_END

#include <rttr/registration>

RTTR_REGISTRATION {
    rttr::registration::class_<Volcano::OpenGL::Renderer>("VolcanoOpenGL")(
        rttr::metadata("Name", "OpenGL"),
        rttr::metadata("Description", "OpenGL Graphics Renderer.")
    ).constructor<Volcano::World::Scene&>();
}
