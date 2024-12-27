//
//
#ifndef VOLCANO_WORLD_SCREEN_H
#define VOLCANO_WORLD_SCREEN_H

#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class Screen: public Transformable {
    Q_OBJECT
    Q_PROPERTY(QVariant source READ source WRITE setSource NOTIFY sourceChanged FINAL)

public:
    using Status = QQmlComponent::Status;

public:
    Screen(QObject* parent = nullptr);
    ~Screen() override;

public:
    Status status() const {
        if (component_ != nullptr) {
            return component_->status();
        }
        return Status::Null;
    }

    bool isReady() const {
        return (status() == Status::Ready);
    }

    QQuickItem* item() {
        return item_;
    }

    const QVariant& source() const {
        return source_;
    }

    void setSource(const QVariant& v);

signals:
    void sourceChanged(const QVariant& v);

private slots:
    void onComponentStatusChanged(QQmlComponent::Status st);

private:
    QVariant source_;
    QQmlComponent* component_;
    QQuickItem* item_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_SCREEN_H
