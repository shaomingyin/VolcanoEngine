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
    Q_PROPERTY(int flags READ flags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    enum {
        FlagEnabled = 0x1,
        FlagVisible = 0x2
    };

public:
    Object(void);
    virtual ~Object(void);

public:
    int flags(void) const;
    void setFlags(int v);
    Q_INVOKABLE bool isEnabled(void) const;
    Q_INVOKABLE bool isVisible(void) const;
    Object *parent(void);
    Object *firstChild(void);
    Object *prevSibling(void);
    Object *nextSibling(void);
    void addChild(Object *obj);
    void removeChild(Object *obj);
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &v);
    const QQuaternion &rotation(void) const;
    void setRotation(const QQuaternion &v);

signals:
    void flagsChanged(int v);
    void positionChanged(const QVector3D &v);
    void scaleChanged(const QVector3D &v);
    void rotationChanged(const QQuaternion &v);

protected:
    void setParent(Object *newParent);

private:
    int m_flags;
    Object *m_parent;
    Object *m_firstChild;
    Object *m_prevSibling;
    Object *m_nextSibling;
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
};

VOLCANO_INLINE int Object::flags(void) const
{
    return m_flags;
}

VOLCANO_INLINE void Object::setFlags(int v)
{
    if (m_flags != v)
    {
        m_flags = v;
        flagsChanged(v);
    }
}

VOLCANO_INLINE bool Object::isEnabled(void) const
{
    return m_flags & FlagEnabled;
}

VOLCANO_INLINE bool Object::isVisible(void) const
{
    return m_flags & FlagVisible;
}

VOLCANO_INLINE Object *Object::parent(void)
{
    return m_parent;
}

VOLCANO_INLINE Object *Object::firstChild(void)
{
    return m_firstChild;
}

VOLCANO_INLINE Object *Object::prevSibling(void)
{
    return m_prevSibling;
}

VOLCANO_INLINE Object *Object::nextSibling(void)
{
    return m_nextSibling;
}

VOLCANO_INLINE void Object::addChild(Object *obj)
{
    Q_ASSERT(obj != nullptr);

    obj->setParent(this);
}

VOLCANO_INLINE void Object::removeChild(Object *obj)
{
    Q_ASSERT(obj != nullptr);
    Q_ASSERT(obj->parent() == this);

    obj->setParent(nullptr);
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
        rotationChanged(v);
    }
}

VOLCANO_END

#endif // VOLCANO_OBJECT_H
