//
//
#ifndef VOLCANO_RESOURCE_COMMON_H
#define VOLCANO_RESOURCE_COMMON_H

#include <QIODevice>
#include <QDataStream>

#include <Volcano/Common.h>

#define VOLCANO_RESOURCE_BEGIN VOLCANO_BEGIN namespace Resource {
#define VOLCANO_RESOURCE_END } VOLCANO_END

VOLCANO_RESOURCE_BEGIN

struct Vector2f {
    float v[2];
};

QDataStream &operator<<(QDataStream &s, const Vector2f& v);
QDataStream &operator>>(QDataStream &s, Vector2f& v);

struct Vector3f {
    float v[3];
};

QDataStream &operator<<(QDataStream &s, const Vector3f& v);
QDataStream &operator>>(QDataStream &s, Vector3f& v);

struct Vector4f {
    float v[4];
};

QDataStream &operator<<(QDataStream &s, const Vector4f& v);
QDataStream &operator>>(QDataStream &s, Vector4f& v);

struct Matrix2x2f {
    float v[2][2];
};

QDataStream &operator<<(QDataStream &s, const Matrix2x2f& v);
QDataStream &operator>>(QDataStream &s, Matrix2x2f& v);

struct Matrix3x3f {
    float v[3][3];
};

QDataStream &operator<<(QDataStream &s, const Matrix3x3f& v);
QDataStream &operator>>(QDataStream &s, Matrix3x3f& v);

struct Matrix4x4f {
    float v[4][4];
};

QDataStream &operator<<(QDataStream &s, const Matrix4x4f& v);
QDataStream &operator>>(QDataStream &s, Matrix4x4f& v);

enum class DetailLevel: quint8 {
    Minimal = 0,
    Low,
    Mid,
    High,
    Ultra,
    Max // Donot use!
};

QDataStream &operator<<(QDataStream &s, DetailLevel v);
QDataStream &operator>>(QDataStream &s, DetailLevel& v);

VOLCANO_RESOURCE_END

#endif // VOLCANO_RESOURCE_COMMON_H
