//
//
#ifndef VOLCANO_COMMON_H
#define VOLCANO_COMMON_H

#include <QtGlobal>
#include <QDebug>
#include <QQmlEngine>

#include <Volcano/Config.h>

#if defined(VOLCANO)
#   define VOLCANO_API Q_DECL_EXPORT
#else
#   define VOLCANO_API /*Q_DECL_IMPORT*/
#endif

#define VOLCANO_INLINE inline

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

struct Vertex {
    struct { float x, y, z; } position;
    struct { float x, y, z; } normal;
    struct { float u, v; } texCoord;
};

typedef quint16 VertexIndex;

template <typename T>
void registerType(const char *uri, const char *qmlName)
{
    qmlRegisterType<T>(uri, VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, qmlName);
}

VOLCANO_END

#endif // VOLCANO_COMMON_H
