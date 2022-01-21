//
//
#ifndef VOLCANO_TRANSFORM_HPP
#define VOLCANO_TRANSFORM_HPP

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Transform {
public:
    Transform(void);
    Transform(const Transform &that);
    Transform(const QVector3D &translate, const QVector3D &scale, const QQuaternion &rotation);

public:
    void reset(void);
    const QVector3D &translate(void) const;
    void setTranslate(const QVector3D &v);
    void setTranslate(float x, float y, float z);
    void setTranslateOffset(const QVector3D &v);
    void setTrnaslateOffset(float x, float y, float z);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &v);
    void setScale(float x, float y, float z);
    void setScaleOffset(const QVector3D &v);
    void setScaleOffset(float x, float y, float z);
    const QQuaternion &rotation(void) const;
    void setRotation(const QQuaternion &v);
    void setRotation(float angle, const QVector3D &axis);
    void setRotation(float angle, float x, float y, float z);
    void setRotationOffset(const QQuaternion &v);
    void setRotationOffset(float angle, const QVector3D &axis);
    void setRotationOffset(float angle, float x, float y, float z);
    void buildMatrix4x4(QMatrix4x4 &out);
    Transform &operator=(const Transform &that);
    bool operator==(const Transform &that) const;
    bool operator!=(const Transform &that) const;

private:
    QVector3D m_translate;
    QVector3D m_scale;
    QQuaternion m_rotation;
};

VOLCANO_END

#endif // VOLCANO_TRANSFORM_HPP
