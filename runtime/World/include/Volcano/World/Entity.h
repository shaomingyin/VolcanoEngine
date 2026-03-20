//
//
#ifndef VOLCANO_WORLD_ENTITY_H
#define VOLCANO_WORLD_ENTITY_H

#include <QList>
#include <QMatrix4x4>
#include <QQmlListProperty>
#include <QObject>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transform.h>
#include <Volcano/World/EntityPhysics.h>

VOLCANO_WORLD_BEGIN

class Entity: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(Transform* transform READ transform)
    Q_PROPERTY(QQmlListProperty<Component> components READ qmlComponents)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject* parent = nullptr);
    ~Entity() override;

public:
    Transform* transform() noexcept {
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

    const QObjectList& components() const {
        return components_;
    }

    void appendComponent(QObject* p);
    QObject* componentAt(qsizetype i);
    void clearComponents();
    qsizetype componentCount();
    void removeLastComponent();
    void replaceComponent(qsizetype i, QObject* p);
    QQmlListProperty<QObject> qmlComponents();

signals:
    void enabledChanged(bool v);
    void componentAdded(QObject* p);
    void componentRemoved(QObject* p);

private:
    bool enabled_;
    Transform transform_;
    QObjectList components_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENTITY_H
