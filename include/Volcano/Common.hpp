//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <Volcano/Config.hpp>

#define VOLCANO_INLINE inline

#ifndef VOLCANO_DEBUG
#   define QT_NO_DEBUG_OUTPUT
#endif

#include <QtGlobal>
#include <QtConcurrent>

#include <QDebug>
#include <QQmlEngine>

#ifdef VOLCANO
#   define VOLCANO_API Q_DECL_EXPORT
#else
#   define VOLCANO_API //Q_DECL_IMPORT
#endif

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

template <typename T>
static void registerType(const char *qmlName)
{
    qmlRegisterType<T>("Volcano", VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName);
}

template <typename T>
static void registerUncreatableType(const char *qmlName)
{
    qmlRegisterUncreatableType<T>("Volcano", VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName,
        QString("Cannot create instance of 'Volcano.%1").arg(qmlName));
}

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
