//
//
#ifndef VOLCANO_GAME_COMMON_HPP
#define VOLCANO_GAME_COMMON_HPP

#include <string>

#include <Volcano/Common.hpp>

#ifdef VOLCANO_DEBUG
#	define ENTT_ASSERT VOLCANO_ASSERT
#else
#	define ENTT_DISABLE_ASSERT
#endif

#include <entt/entt.hpp>

#define VOLCANO_GAME_BEGIN VOLCANO_BEGIN namespace Game {
#define VOLCANO_GAME_END } VOLCANO_END

VOLCANO_GAME_BEGIN

struct Dynamic final {
	float mass;
	float x;
	float y;
	float z;
};

struct Position final {
	float x;
	float y;
	float z;
};

struct Scale final {
	float x;
	float y;
	float z;
};

struct Rotation final {
	float angle;
	float x;
	float y;
	float z;
};

struct Velocity final {
	float x;
	float y;
	float z;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMMON_HPP
