//
//
#ifndef VOLCANO_WORLD_ENTITY_H
#define VOLCANO_WORLD_ENTITY_H

#include <QMatrix4x4>
#include <QList>
#include <QObject>
#include <QQmlListProperty>

#include <Volcano/World/Common.h>
#include <Volcano/World/Component.h>

VOLCANO_WORLD_BEGIN

class Entity: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enable READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QQmlListProperty<Component> components READ qmlComponents)
    Q_CLASSINFO("DefaultProperty", "components")

public:
    Entity(QObject* parent = nullptr);

public:
    bool isEnabled() const {
        return enabled_;
    }

    void setEnabled(bool v) {
        if (enabled_ != v) {
            enabled_ = v;
            emit enabledChanged(v);
        }
    }

    Q_INVOKABLE void enable() {
        setEnabled(true);
    }

    Q_INVOKABLE void disable() {
        setEnabled(false);
    }

    void update(Duration elapsed) {
        if (enabled_) {
            tick(elapsed);
        }
    }

    const QMatrix4x4& transform() const {
        return transform_;
    }

    void setTransform(const QMatrix4x4& v) {
        if (!qFuzzyCompare(transform_, v)) {
            transform_ = v;
            emit transformChanged(v);
        }
    }

    Q_INVOKABLE void resetTransform() {
        transform_.setToIdentity();
    }

    const QList<Component*>& components() const {
        return components_;
    }

    void appendComponent(Component* p);
    Component* componentAt(qsizetype i);
    const Component* componentAt(qsizetype i) const;
    void clearComponents();
    qsizetype componentCount() const;
    void removeLastComponent();
    void replaceComponent(qsizetype i, Component* p);
    QQmlListProperty<Component> qmlComponents();

signals:
    void enabledChanged(bool v);
    void transformChanged(const QMatrix4x4& v);
    void componentAdded(Component* component);
    void componentRemoved(Component* component);

protected:
    virtual void tick(Duration elapsed);

private:
    bool enabled_;
    QMatrix4x4 transform_;
    QList<Component*> components_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_ENTITY_H
