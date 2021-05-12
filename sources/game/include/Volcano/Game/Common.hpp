//
//
#ifndef VOLCANO_GAME_COMMON_HPP
#define VOLCANO_GAME_COMMON_HPP

#include <string>

#define FLECS_CUSTOM_BUILD

#define FLECS_BULK
#define FLECS_STATS
#define FLECS_DIRECT_ACCESS
#define FLECS_MODULE
#define FLECS_READER_WRITER
#define FLECS_SNAPSHOT
#define FLECS_SYSTEM
#define FLECS_PIPELINE
#define FLECS_TIMER

#ifdef VOLCANO_DEBUG
#	define FLECS_DBG
#endif

#include <flecs.h>

#include <Volcano/Common.hpp>

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
