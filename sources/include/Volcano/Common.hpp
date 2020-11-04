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

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

template <typename T>
static void registerType(const char *qmlName, const char *qmlUrl = nullptr)
{
    QString url;

    if (qmlUrl != nullptr)
        url = QString("Volcano.") + qmlUrl;
    else
        url = QString("Volcano");

    qmlRegisterType<T>(qPrintable(url), VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName);
}

template <typename T>
static void registerUncreatableType(const char *qmlName, const char *qmlUrl = nullptr)
{
    QString url;

    if (qmlUrl != nullptr)
        url = QString("Volcano.") + qmlUrl;
    else
        url = QString("Volcano");

    qmlRegisterUncreatableType<T>(qPrintable(url), VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName,
        QString("Cannot create instance of 'Volcano.%1").arg(qmlName));
}

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
