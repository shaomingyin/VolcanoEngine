//
//
#ifndef VOLCANO_NET_COMMON_HPP
#define VOLCANO_NET_COMMON_HPP

#include <enet/enet.h>

#include <Volcano/Common.hpp>

#define VOLCANO_NET_BEGIN VOLCANO_BEGIN namespace Net {
#define VOLCANO_NET_END } VOLCANO_END

VOLCANO_NET_BEGIN

constexpr quint16 DEFAULT_PORT = 7788;
constexpr int DEFAULT_MAX_SESSION = 16;

VOLCANO_NET_END

#endif // VOLCANO_NET_COMMON_HPP
