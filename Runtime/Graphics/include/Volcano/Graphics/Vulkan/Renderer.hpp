//
//
#ifndef VOLCANO_GRAPHICS_VULKAN_RENDERER_HPP
#define VOLCANO_GRAPHICS_VULKAN_RENDERER_HPP

#include <Volcano/Graphics/Renderer.hpp>
#include <Volcano/Graphics/Vulkan/Common.hpp>

VOLCANO_GRAPHICS_VULKAN_BEGIN

class VOLCANO_GRAPHICS_API Renderer: public Graphics::Renderer {
    Q_OBJECT

public:
    Renderer(QObject* parent = nullptr);
    ~Renderer() override = default;

protected:
    bool doInit(int width, int height) override;
};

VOLCANO_GRAPHICS_VULKAN_END

#endif // VOLCANO_GRAPHICS_VULKAN_RENDERER_HPP
