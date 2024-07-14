//
//
#ifndef VOLCANO_LAUNCHER_VULKANWINDOW_HPP
#define VOLCANO_LAUNCHER_VULKANWINDOW_HPP

#include <QVulkanWindow>

#include <Volcano/Graphics/Vulkan/Renderer.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class VulkanWindow: public QVulkanWindow {
    Q_OBJECT

public:
    VulkanWindow();
    ~VulkanWindow() override = default;

private:
    Graphics::Vulkan::Renderer renderer_;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_VULKANWINDOW_HPP
