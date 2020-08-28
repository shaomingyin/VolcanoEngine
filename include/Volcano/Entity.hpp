//
//
#ifndef VOLCANO_ENTITY_HPP
#define VOLCANO_ENTITY_HPP

#include <QList>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/Common.hpp>
#include <Volcano/Component.hpp>
#include <Volcano/OpenGL/View.hpp>

VOLCANO_BEGIN

typedef QList<Component *> ComponentList;

class VOLCANO_API Entity: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)

public:
    Entity(QObject *parent = nullptr);
	~Entity(void) override;

public:
    const QString &name(void) const;
    void setName(const QString &v);
    bool enabled(void) const;
    void setEnabled(bool v);
    bool visible(void) const;
    void setVisible(bool v);
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &r);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &r);
    const QQuaternion &rotation(void) const;
    void setRotation(const QQuaternion &r);
    void addComponent(Component *p);
    void removeComponent(Component *p);
    virtual void update(float elapsed);
    virtual void draw(OpenGL::View &view);

signals:
    void nameChanged(const QString &name);
    void enabledChanged(bool v);
    void visibleChanged(bool v);
    void positionChanged(const QVector3D &r);
    void scaleChanged(const QVector3D &r);
    void rotationChanged(const QQuaternion &r);
    void componentAdded(Component *p);
    void componentRemoved(Component *p);

protected:
    ComponentList m_componentList;

private:
    QString m_name;
    bool m_enabled;
    bool m_visible;
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
};

VOLCANO_INLINE const QString &Entity::name(void) const
{
    return m_name;
}

VOLCANO_INLINE void Entity::setName(const QString &v)
{
    if (m_name != v)
    {
        m_name = v;
        nameChanged(v);
    }
}

VOLCANO_INLINE bool Entity::enabled(void) const
{
    return m_enabled;
}

VOLCANO_INLINE void Entity::setEnabled(bool v)
{
    if (m_enabled != v)
    {
        m_enabled = v;
        enabledChanged(v);
    }
}

VOLCANO_INLINE bool Entity::visible(void) const
{
    return m_visible;
}

VOLCANO_INLINE void Entity::setVisible(bool v)
{
    if (m_visible != v)
    {
        m_visible = v;
        visibleChanged(v);
    }
}

VOLCANO_INLINE void Entity::addComponent(Component *p)
{
    Q_ASSERT(!m_componentList.contains(p));
    m_componentList.append(p);
    componentAdded(p);
}

VOLCANO_INLINE void Entity::removeComponent(Component *p)
{
    Q_ASSERT(m_componentList.contains(p));
    m_componentList.removeAll(p);
    componentRemoved(p);
}

VOLCANO_INLINE const QVector3D &Entity::position(void) const
{
	return m_position;
}

VOLCANO_INLINE void Entity::setPosition(const QVector3D &r)
{
    if (m_position != r)
    {
        m_position = r;
        positionChanged(r);
    }
}

VOLCANO_INLINE const QVector3D &Entity::scale(void) const
{
	return m_scale;
}

VOLCANO_INLINE void Entity::setScale(const QVector3D &r)
{
    if (m_scale != r)
    {
        m_scale = r;
        scaleChanged(r);
    }
}

VOLCANO_INLINE const QQuaternion &Entity::rotation(void) const
{
	return m_rotation;
}

VOLCANO_INLINE void Entity::setRotation(const QQuaternion &r)
{
    if (m_rotation != r)
    {
        m_rotation = r;
        rotationChanged(r);
    }
}

VOLCANO_END

#endif // VOLCANO_ENTITY_HPP
