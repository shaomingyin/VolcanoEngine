//
//
#include <Volcano/Resource/Common.h>

VOLCANO_RESOURCE_BEGIN

// Vector2f

QDataStream &operator<<(QDataStream &s, const Vector2f& v) {
    s << v.v[0];
    s << v.v[1];
    return s;
}

QDataStream &operator>>(QDataStream &s, Vector2f& v) {
    s >> v.v[0];
    s >> v.v[1];
    return s;
}

// Vector3f

QDataStream &operator<<(QDataStream &s, const Vector3f& v) {
    s << v.v[0];
    s << v.v[1];
    s << v.v[2];
    return s;
}

QDataStream &operator>>(QDataStream &s, Vector3f& v) {
    s >> v.v[0];
    s >> v.v[1];
    s >> v.v[2];
    return s;
}

// Vector4f

QDataStream &operator<<(QDataStream &s, const Vector4f& v) {
    s << v.v[0];
    s << v.v[1];
    s << v.v[2];
    s << v.v[3];
    return s;
}

QDataStream &operator>>(QDataStream &s, Vector4f& v) {
    s >> v.v[0];
    s >> v.v[1];
    s >> v.v[2];
    s >> v.v[3];
    return s;
}

// Matrix2x2f

QDataStream &operator<<(QDataStream &s, const Matrix2x2f& v) {
    s << v.v[0][0];
    s << v.v[0][1];
    s << v.v[1][0];
    s << v.v[2][1];
    return s;
}

QDataStream &operator>>(QDataStream &s, Matrix2x2f& v) {
    s >> v.v[0][0];
    s >> v.v[0][1];
    s >> v.v[1][0];
    s >> v.v[2][1];
    return s;
}

// Matrix3x3f

QDataStream &operator<<(QDataStream &s, const Matrix3x3f& v) {
    s << v.v[0][0];
    s << v.v[0][1];
    s << v.v[0][2];
    s << v.v[1][0];
    s << v.v[1][1];
    s << v.v[1][2];
    s << v.v[2][0];
    s << v.v[2][1];
    s << v.v[2][2];
    return s;
}

QDataStream &operator>>(QDataStream &s, Matrix3x3f& v) {
    s >> v.v[0][0];
    s >> v.v[0][1];
    s >> v.v[0][2];
    s >> v.v[1][0];
    s >> v.v[1][1];
    s >> v.v[1][2];
    s >> v.v[2][0];
    s >> v.v[2][1];
    s >> v.v[2][2];
    return s;
}

// Matrix4x4f

QDataStream &operator<<(QDataStream &s, const Matrix4x4f& v) {
    s << v.v[0][0];
    s << v.v[0][1];
    s << v.v[0][2];
    s << v.v[0][3];
    s << v.v[1][0];
    s << v.v[1][1];
    s << v.v[1][2];
    s << v.v[1][3];
    s << v.v[2][0];
    s << v.v[2][1];
    s << v.v[2][2];
    s << v.v[2][3];
    s << v.v[3][0];
    s << v.v[3][1];
    s << v.v[3][2];
    s << v.v[3][3];
    return s;
}

QDataStream &operator>>(QDataStream &s, Matrix4x4f& v) {
    s >> v.v[0][0];
    s >> v.v[0][1];
    s >> v.v[0][2];
    s >> v.v[0][3];
    s >> v.v[1][0];
    s >> v.v[1][1];
    s >> v.v[1][2];
    s >> v.v[1][3];
    s >> v.v[2][0];
    s >> v.v[2][1];
    s >> v.v[2][2];
    s >> v.v[2][3];
    s >> v.v[3][0];
    s >> v.v[3][1];
    s >> v.v[3][2];
    s >> v.v[3][3];
    return s;
}

VOLCANO_RESOURCE_END
