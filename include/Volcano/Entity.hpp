//
//
#ifndef VOLCANO_ENTITY_HPP
#define VOLCANO_ENTITY_HPP

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

class VOLCANO_API Entity: public Node
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)

public:
	Entity(Node *parent = nullptr);
	~Entity(void) override;

public:
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &v);
    const QQuaternion &rotation(void) const;
    void setRotation(const QQuaternion &v);
    QMatrix4x4 transform(void) const;

signals:
    void positionChanged(const QVector3D &v);
    void scaleChanged(const QVector3D &v);
    void rotationChanged(const QQuaternion &v);

protected:

private:
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
};

VOLCANO_INLINE const QVector3D &Entity::position(void) const
{
	return m_position;
}

VOLCANO_INLINE void Entity::setPosition(const QVector3D &v)
{
	m_position = v;
}

VOLCANO_INLINE const QVector3D &Entity::scale(void) const
{
	return m_scale;
}

VOLCANO_INLINE void Entity::setScale(const QVector3D &v)
{
	m_scale = v;
}

VOLCANO_INLINE const QQuaternion &Entity::rotation(void) const
{
	return m_rotation;
}

VOLCANO_INLINE void Entity::setRotation(const QQuaternion &v)
{
	m_rotation = v;
}

VOLCANO_INLINE QMatrix4x4 Entity::transform(void) const
{
    QMatrix4x4 m;

    return m;
}

VOLCANO_END

#endif // VOLCANO_ENTITY_HPP
