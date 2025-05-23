//
//
#ifndef VOLCANO_PHYSICS_WORLD_H
#define VOLCANO_PHYSICS_WORLD_H

#include <Volcano/World/Scene.h>
#include <Volcano/Physics/Common.h>

VOLCANO_PHYSICS_BEGIN

template <typename T>
concept World_construct = requires(Volcano::World::Scene & scene) {
	T(scene);
};

template <typename T>
concept World_isEnabled = requires(const T & v) {
	{ v.isEnabled() } -> std::convertible_to<bool>;
};

template <typename T>
concept World_enable = requires(T & v) {
	v.enable();
};

template <typename T>
concept World_disable = requires(T & v) {
	v.disable();
};

template <typename T>
concept World_update = requires(T & v, Duration elapsed) {
	v.update(elapsed);
};

template <typename T>
concept World =
	World_construct<T> &&
	World_isEnabled<T> &&
	World_enable<T> &&
	World_disable<T> &&
	World_update<T>;

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_WORLD_H
