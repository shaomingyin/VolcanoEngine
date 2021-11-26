//
//
#ifndef VOLCANO_SYSTEM_COMMON_HPP
#define VOLCANO_SYSTEM_COMMON_HPP

#include <QString>
#include <QIODevice>

#include <Volcano/Common.hpp>

#define VOLCANO_SYSTEM_BEGIN VOLCANO_BEGIN namespace System {
#define VOLCANO_SYSTEM_END } VOLCANO_END

VOLCANO_SYSTEM_BEGIN

QAbstractSocket *openSocket(QUrl url);

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_COMMON_HPP
