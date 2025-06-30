//
//
#ifndef VOLCANO_GRAPHICS_DEVICE_H
#define VOLCANO_GRAPHICS_DEVICE_H

#include <concepts>
#include <functional>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Target.h>
#include <Volcano/Graphics/Window.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

class Device {
    RTTR_ENABLE()

public:
    Device() = default;
    virtual ~Device() = default;

public:
    virtual Window* createWindow(const std::string& title, int width, int height) = 0;
    virtual Texture* createTexture(int width, int height) = 0;
    virtual void render(const View& view) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DEVICE_H
