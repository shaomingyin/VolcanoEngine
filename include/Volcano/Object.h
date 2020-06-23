//
//
#ifndef VOLCANO_OBJECT_H
#define VOLCANO_OBJECT_H

#include <QObject>
#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class VOLCANO_API Object: public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enable READ enable WRITE setEnable NOTIFY enableChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    Object(QObject *parent = nullptr);
    virtual ~Object(void);

public:
    bool enable(void) const;
    void setEnable(bool v);
    bool visible(void) const;
    void setVisible(bool v);
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &v);
    const QQuaternion &rotation(void) const;
    void setRotation(const QQuaternion &v);
    const QMatrix4x4 &transform(void);
    QMatrix4x4 globalTransform(void);

signals:
    void enableChanged(bool v);
    void visibleChanged(bool v);
    void positionChanged(const QVector3D &v);
    void scaleChanged(const QVector3D &v);
    void rotationChanged(const QQuaternion &v);

private:
    enum
    {
        FlagEnable = 0x1,
        FlagVisible = 0x2,
        FlagDirty = 0x4
    };

    int m_flags;
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
    QMatrix4x4 m_transform;
};

VOLCANO_INLINE bool Object::enable(void) const
{
    return (m_flags & FlagEnable);
}

VOLCANO_INLINE void Object::setEnable(bool v)
{
    if (enable() != v)
    {
        if (v)
            m_flags |= FlagEnable;
        else
            m_flags &= ~FlagEnable;
        enableChanged(v);
    }
}

VOLCANO_INLINE bool Object::visible(void) const
{
    return (m_flags & FlagVisible);
}

VOLCANO_INLINE void Object::setVisible(bool v)
{
    if (visible() != v)
    {
        if (v)
            m_flags |= FlagVisible;
        else
            m_flags &= ~FlagVisible;

        visibleChanged(v);
    }
}

VOLCANO_INLINE const QVector3D &Object::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void Object::setPosition(const QVector3D &v)
{
    if (m_position != v)
    {
        m_position = v;
        m_flags |= FlagDirty;
        positionChanged(v);
    }
}

VOLCANO_INLINE const QVector3D &Object::scale(void) const
{
    return m_scale;
}

VOLCANO_INLINE void Object::setScale(const QVector3D &v)
{
    if (m_scale != v)
    {
        m_scale = v;
        m_flags |= FlagDirty;
        scaleChanged(v);
    }
}

VOLCANO_INLINE const QQuaternion &Object::rotation(void) const
{
    return m_rotation;
}

VOLCANO_INLINE void Object::setRotation(const QQuaternion &v)
{
    if (m_rotation != v)
    {
        m_rotation = v;
        m_flags |= FlagDirty;
        rotationChanged(v);
    }
}

VOLCANO_INLINE const QMatrix4x4 &Object::transform(void)
{
    if (m_flags & FlagDirty)
    {
        m_transform.setToIdentity();
        m_transform.scale(scale());
        m_transform.rotate(rotation());
        m_transform.translate(position());
        m_flags &= ~FlagDirty;
    }
    return m_transform;
}

VOLCANO_END

#endif // VOLCANO_OBJECT_H
