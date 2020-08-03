//
//
#ifndef VOLCANO_COMMON_HPP
#define VOLCANO_COMMON_HPP

#include <QtGlobal>
#include <QDebug>
#include <QVector3D>

#include <Volcano/Config.hpp>

#define VOLCANO_INLINE inline

#ifdef VOLCANO
#   define VOLCANO_API Q_DECL_EXPORT
#else
#   define VOLCANO_API Q_DECL_IMPORT
#endif

#define VOLCANO_BEGIN namespace Volcano {
#define VOLCANO_END }

VOLCANO_BEGIN

struct Vertex {
    QVector3D position;
    QVector3D normal;
    QVector3D texCoord;
};

typedef quint32 VertexIndex;

VOLCANO_END

#endif // VOLCANO_COMMON_HPP
