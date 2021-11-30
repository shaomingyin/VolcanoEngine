//
//
#ifndef VOLCANO_SYSTEM_COMMON_HPP
#define VOLCANO_SYSTEM_COMMON_HPP

#include <enet/enet.h>

#include <Volcano/Common.hpp>

#define VOLCANO_SYSTEM_BEGIN VOLCANO_BEGIN namespace System {
#define VOLCANO_SYSTEM_END } VOLCANO_END

VOLCANO_SYSTEM_BEGIN

static constexpr quint16 DEFAULT_PORT = 7788;

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_COMMON_HPP
