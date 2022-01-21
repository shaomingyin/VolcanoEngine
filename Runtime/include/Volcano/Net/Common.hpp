//
//
#ifndef VOLCANO_NET_COMMON_HPP
#define VOLCANO_NET_COMMON_HPP

#include <enet/enet.h>

#include <Volcano/Common.hpp>

#define VOLCANO_NET_BEGIN VOLCANO_BEGIN namespace Net {
#define VOLCANO_NET_END } VOLCANO_END

VOLCANO_NET_BEGIN

constexpr static quint8 VERSION = 1;
constexpr static QDataStream::Version STREAM_VERSION = QDataStream::Qt_6_0;
constexpr static QDataStream::ByteOrder STREAM_BYTE_ORDER = QDataStream::LittleEndian;
constexpr static quint16 DEFAULT_PORT = 7788;

enum class Type: quint8 {
    Invalid = 0,
    Ping,
    Pong
};

VOLCANO_NET_END

#endif // VOLCANO_NET_COMMON_HPP
