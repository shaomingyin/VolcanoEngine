//
//
#ifndef VOLCANO_GRAPHICS_TRANSFORM_HPP
#define VOLCANO_GRAPHICS_TRANSFORM_HPP

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Transform {
public:
    Transform(void);
    Transform(const Transform &that);
    Transform(Transform &&that);
    Transform(const QVector3D &translate, const QVector3D &scale, const QQuaternion &rotation);

public:
    void reset(void);
    void translate(const QVector3D &v);
    void translate(float x, float y, float z);
    void translateTo(const QVector3D &v);
    void trnaslateTo(float x, float y, float z);
    void scale(const QVector3D &v);
    void scale(float x, float y, float z);
    void scaleTo(const QVector3D &v);
    void scaleTo(float x, float y, float z);
    void rotate(const QQuaternion &v);
    void rotate(float angle, const QVector3D &axis);
    void rotate(float angle, float x, float y, float z);
    void rotateTo(const QQuaternion &v);
    void rotateTo(float angle, const QVector3D &axis);
    void rotateTo(float angle, float x, float y, float z);
    void toMatrix(QMatrix4x4 &out);
    Transform &operator=(const Transform &that);
    Transform &operator=(Transform &&that);
    bool operator==(const Transform &that) const;
    bool operator!=(const Transform &that) const;

private:
    QVector3D m_translate;
    QVector3D m_scale;
    QQuaternion m_rotation;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TRANSFORM_HPP
