//
//
#ifndef VOLCANO_GRAPHICS_TARGET_H
#define VOLCANO_GRAPHICS_TARGET_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

template <typename T>
concept Target_isEnabled = requires(const T& v) {
	{ v.isEnabled() } -> std::convertible_to<bool>;
};

template <typename T>
concept Target_enable = requires(T& v) {
	{ v.enable() } -> std::same_as<void>;
};

template <typename T>
concept Target_disable = requires(T& v) {
	{ v.disable() } -> std::same_as<void>;
};

template <typename T>
concept Target_width = requires(const T& v) {
	{ v.width() } -> std::convertible_to<int>;
};

template <typename T>
concept Target_height = requires(const T & v) {
	{ v.height() } -> std::convertible_to<int>;
};

template <typename T>
concept Target_resize = requires(T & v, int width, int height) {
	{ v.resize(width, height) } -> std::same_as<void>;
};

template <typename T>
concept Target =
	Target_isEnabled<T> &&
	Target_enable<T> &&
	Target_disable<T> &&
	Target_width<T> &&
	Target_height<T> &&
	Target_resize<T>;

template <Target target_type>
struct TargetType {
	using Type = target_type;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TARGET_H
