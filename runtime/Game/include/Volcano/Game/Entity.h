//
//
#ifndef VOLCANO_GAME_ENTITY_H
#define VOLCANO_GAME_ENTITY_H

#include <QList>
#include <QMatrix4x4>
#include <QQmlListProperty>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Component.h>
#include <Volcano/Game/Transform.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class Entity: public Object {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(Transform* transform READ transform)
    Q_PROPERTY(QQmlListProperty<Component> components READ qmlComponents)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject* parent = nullptr);
    ~Entity() override;

public:
    Transform* transform() {
        return &transform_;
    }

    bool isEnabled() const {
        return enabled_;
    }

    void setEnabled(bool v) {
        if (enabled_ != v) {
            enabled_ = v;
            emit enabledChanged(v);
        }
    }

    const QList<Component*>& components() const {
        return components_;
    }

    void appendComponent(Component* p);
    Component* componentAt(qsizetype i);
    void clearComponents();
    qsizetype componentCount();
    void removeLastComponent();
    void replaceComponent(qsizetype i, Component* p);
    QQmlListProperty<Component> qmlComponents();

    friend QDataStream& operator<<(QDataStream& s, const Entity& v);
    friend QDataStream& operator>>(QDataStream& s, Entity& v);

signals:
    void enabledChanged(bool v);
    void componentAdded(Component* p);
    void componentRemoved(Component* p);

private:
    bool enabled_;
    Transform transform_;
    QList<Component*> components_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_H
