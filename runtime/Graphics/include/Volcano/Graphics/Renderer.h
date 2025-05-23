//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <concepts>
#include <functional>

#include <Volcano/World/Scene.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Target.h>
#include <Volcano/Graphics/Window.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

template <typename T>
concept WindowPointer = std::is_pointer_v<T> && Window<std::remove_pointer_t<T>>;

template <typename T>
concept TargetPointer = std::is_pointer_v<T> && Target<std::remove_pointer_t<T>>;

template <typename T>
concept Renderer_constructor = requires(World::Scene & scene) { T(scene); };

template <typename R>
concept Renderer_createWindow = requires(R& r, const std::string& name, int width, int height) {
    { r.createWindow(name, width, height) } -> WindowPointer;
};

template <typename T>
concept Renderer_createTexture = requires(T& v, int width, int height) {
    { v.createTexture(width, height) } -> TargetPointer;
};

template <typename R>
concept Renderer_render = requires(R& r, const View& view, Duration elapsed) {
    r.render(view, elapsed);
};

template <typename T>
concept Renderer =
    Renderer_constructor<T> &&
    Renderer_createWindow<T> &&
    Renderer_createTexture<T> &&
    Renderer_render<T>;

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
