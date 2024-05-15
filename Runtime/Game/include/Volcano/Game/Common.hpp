//
//
#ifndef VOLCANO_GAME_COMMON_HPP
#define VOLCANO_GAME_COMMON_HPP

#include <QtQml>

#include <Volcano/Common.hpp>

#ifdef VOLCANO_GAME
#   define VOLCANO_GAME_API Q_DECL_EXPORT
#else
#   define VOLCANO_GAME_API Q_DECL_IMPORT
#endif

#define VOLCANO_GAME_BEGIN VOLCANO_BEGIN namespace Game {
#define VOLCANO_GAME_END } VOLCANO_END

#endif // VOLCANO_GAME_COMMON_HPP
