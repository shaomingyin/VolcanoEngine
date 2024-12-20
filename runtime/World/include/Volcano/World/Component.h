//
//
#ifndef VOLCANO_WORLD_COMPONENT_H
#define VOLCANO_WORLD_COMPONENT_H

#include <QObject>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Component: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    Q_INVOKABLE Component(QObject* parent = nullptr)
        : QObject(parent)
        , enabled_(true) {
    }

public:
    bool isEnabled() const {
        return enabled_;
    }

    void setEnabled(bool v) {
        if (v) {
            enable();
        } else {
            disable();
        }
    }

    Q_INVOKABLE void enable() {
        if (!enabled_) {
            enabled_ = true;
            emit enabledChanged(true);
        }
    }

    Q_INVOKABLE void disable() {
        if (enabled_) {
            enabled_ = false;
            emit enabledChanged(false);
        }
    }

signals:
    void enabledChanged(bool v);

private:
    bool enabled_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COMPONENT_H
