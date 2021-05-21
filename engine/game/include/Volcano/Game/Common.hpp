//
//
#ifndef VOLCANO_GAME_COMMON_HPP
#define VOLCANO_GAME_COMMON_HPP

#include <string>

#define NAPI_VERSION 7
#include <node_api.h>

#define NODE_ADDON_API_DISABLE_DEPRECATED
#include <napi.h>

#include <Volcano/Common.hpp>

#ifdef VOLCANO_DEBUG
#	define ENTT_ASSERT VOLCANO_ASSERT
#else
#	define ENTT_DISABLE_ASSERT
#endif

#include <entt/entt.hpp>

#define VOLCANO_GAME_BEGIN VOLCANO_BEGIN namespace Game {
#define VOLCANO_GAME_END } VOLCANO_END

#endif // VOLCANO_GAME_COMMON_HPP
