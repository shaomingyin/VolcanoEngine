//
//
#ifndef VOLCANO_WORLD_COMMON_H
#define VOLCANO_WORLD_COMMON_H

#include <chrono>

#include <QQmlInfo>
#include <QQmlEngine>

#include <Volcano/Common.h>

#define VOLCANO_WORLD_BEGIN VOLCANO_BEGIN namespace World {
#define VOLCANO_WORLD_END } VOLCANO_END

VOLCANO_WORLD_BEGIN

using Clock = std::chrono::steady_clock;

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COMMON_H
