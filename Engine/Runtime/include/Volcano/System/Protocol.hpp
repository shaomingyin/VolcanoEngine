//
//
#ifndef VOLCANO_SYSTEM_PROTOCOL_HPP
#define VOLCANO_SYSTEM_PROTOCOL_HPP

#include <QObject>
#include <QDataStream>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

namespace Protocol {
    constexpr static quint8 VERSION = 1;
    constexpr static QDataStream::Version STREAM_VERSION = QDataStream::Qt_6_0;
    constexpr static QDataStream::ByteOrder STREAM_BYTE_ORDER = QDataStream::LittleEndian;
    constexpr static quint16 DEFAULT_PORT = 7788;

    enum class Type: quint8 {
        Invalid = 0,
        Ping,
        Pong
    };
}

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_PROTOCOL_HPP
