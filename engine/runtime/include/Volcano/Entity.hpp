//
//
#ifndef VOLCANO_ENTITY_HPP
#define VOLCANO_ENTITY_HPP

#include <QList>
#include <QVector3D>
#include <QQuaternion>
#include <QQmlListProperty>

#include <Volcano/Common.hpp>
#include <Volcano/Object.hpp>
#include <Volcano/Component.hpp>

VOLCANO_BEGIN

class Entity: public Object {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(QQmlListProperty<Component> components READ qmlComponents)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject *parent = nullptr);
    ~Entity(void) override;

public:
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &v);
    const QQuaternion &rotation(void);
    void setRotation(const QQuaternion &v);
    const QList<Component *> &components(void) const;
    QQmlListProperty<Component> qmlComponents(void);

signals:
    void positionChanged(const QVector3D &v);
    void scaleChanged(const QVector3D &v);
    void rotationChanged(const QQuaternion &v);
    void componentAdded(Component *component);
    void componentRemoved(Component *component);

private:
    static void appendComponent(QQmlListProperty<Component> *list, Component *component);
    static qsizetype componentCount(QQmlListProperty<Component> *list);
    static Component *componentAt(QQmlListProperty<Component> *list, qsizetype index);
    static void clearComponents(QQmlListProperty<Component> *list);
    static void replaceComponent(QQmlListProperty<Component> *list, qsizetype index, Component *component);
    static void removeLastComponent(QQmlListProperty<Component> *list);

private:
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
    QList<Component *> m_components;
};

VOLCANO_END

#endif // VOLCANO_ENTITY_HPP
