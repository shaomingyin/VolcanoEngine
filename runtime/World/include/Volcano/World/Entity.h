//
//
#ifndef VOLCANO_WORLD_ENTITY_H
#define VOLCANO_WORLD_ENTITY_H

#include <QList>
#include <QMatrix4x4>
#include <QQmlListProperty>

#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>
#include <Volcano/World/Object.h>

VOLCANO_WORLD_BEGIN

class Entity: public Object {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(QMatrix4x4* transform READ transform)
    Q_PROPERTY(QQmlListProperty<Component> components READ qmlComponents)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject* parent = nullptr);
    ~Entity() override;

public:
    QMatrix4x4* transform() {
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

signals:
    void enabledChanged(bool v);
    void componentAdded(Component* p);
    void componentRemoved(Component* p);

private:
    bool enabled_;
    QMatrix4x4 transform_;
    QList<Component*> components_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENTITY_H
