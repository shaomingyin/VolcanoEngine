//
//
#ifndef VOLCANO_GAME_ENTITY_HPP
#define VOLCANO_GAME_ENTITY_HPP

#include <QList>
#include <QVector3D>
#include <QQuaternion>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>
#include <Volcano/Game/Actor.hpp>

VOLCANO_GAME_BEGIN

using ComponentList = QList<Component *>;

class Entity: public Actor {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(qreal mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(QVector3D centerOfMass READ centerOfMass WRITE setCenterOfMass NOTIFY centerOfMassChanged)
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
    qreal mass(void) const;
    void setMass(qreal v);
    const QVector3D &centerOfMass(void) const;
    void setCenterOfMass(const QVector3D &v);
    const ComponentList &components(void) const;
    QQmlListProperty<Component> qmlComponents(void);
    void appendComponent(Component *component);
    qsizetype componentCount(void) const;
    Component *componentAt(qsizetype index);
    const Component *componentAt(qsizetype index) const;
    void clearComponents(void);
    void replaceComponent(qsizetype index, Component *component);
    void removeLastComponent(void);

signals:
    void positionChanged(const QVector3D &v);
    void scaleChanged(const QVector3D &v);
    void rotationChanged(const QQuaternion &v);
    void massChanged(qreal v);
    void centerOfMassChanged(const QVector3D &v);
    void componentAdded(Component *component);
    void componentRemoved(Component *component);

protected:
    void onTick(Duration elapsed) override;
    void onDraw(void) override;

private:
    void handleComponentAdded(Component *component);
    void handleComponentRemoved(Component *component);
    static void qmlAppendComponent(QQmlListProperty<Component> *list, Component *component);
    static qsizetype qmlComponentCount(QQmlListProperty<Component> *list);
    static Component *qmlComponentAt(QQmlListProperty<Component> *list, qsizetype index);
    static void qmlClearComponents(QQmlListProperty<Component> *list);
    static void qmlReplaceComponent(QQmlListProperty<Component> *list, qsizetype index, Component *component);
    static void qmlRemoveLastComponent(QQmlListProperty<Component> *list);

private:
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
    qreal m_mass;
    QVector3D m_centerOfMass;
    ComponentList m_components;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_HPP
