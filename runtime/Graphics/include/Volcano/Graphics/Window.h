//
//
#ifndef VOLCANO_GRAPHICS_WINDOW_H
#define VOLCANO_GRAPHICS_WINDOW_H

#include <string>

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

template <typename T>
concept Window_handle = requires(T& v) {
    { v.handle() } -> std::same_as<SDL_Window*>;
};

template <typename T>
concept Window_show = requires(T & v) {
    { v.show() } -> std::same_as<void>;
};

template <typename T>
concept Window_hide = requires(T & v) {
    { v.hide() } -> std::same_as<void>;
};

template <typename T>
concept Window_isVisible = requires(const T & v) {
    { v.isVisible() } -> std::convertible_to<bool>;
};

template <typename T>
concept Window_event = requires(T & v, const SDL_Event & evt) {
    { v.event(evt) } -> std::same_as<void>;
};

template <typename T>
concept Window =
    Target<T> &&
    Window_handle<T> &&
    Window_show<T> &&
    Window_hide<T> &&
    Window_isVisible<T> &&
    Window_event<T>;

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WINDOW_H
