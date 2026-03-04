//
//
#ifndef VOLCANO_GAME_COMMON_H
#define VOLCANO_GAME_COMMON_H

#include <chrono>

#include <bullet/btBulletDynamicsCommon.h>

#include <QQmlInfo>
#include <QQmlEngine>

#include <Volcano/Common.h>

#define VOLCANO_GAME_BEGIN VOLCANO_BEGIN namespace Game {
#define VOLCANO_GAME_END } VOLCANO_END

VOLCANO_GAME_BEGIN

using Clock = std::chrono::high_resolution_clock;

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMMON_H
