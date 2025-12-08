//
//
#ifndef VOLCANO_GAME_SCREEN_H
#define VOLCANO_GAME_SCREEN_H

#include <memory>

#include <QSize>
#include <QVariant>
#include <QQmlComponent>
#include <QQuickItem>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Transformable.h>

VOLCANO_GAME_BEGIN

class Screen: public Transformable {
    Q_OBJECT
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged FINAL)
    Q_PROPERTY(QVariant source READ source WRITE setSource NOTIFY sourceChanged FINAL)

public:
    using Status = QQmlComponent::Status;

public:
    Screen(QObject* parent = nullptr);

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

    QQuickItem* rootItem() {
        return root_item_;
    }

    const QSize& size() const {
        return size_;
    }

    void setSize(const QSize& v) {
        if (size_ != v) {
            size_ = v;
            emit sizeChanged(v);
        }
    }

    const QVariant& source() const {
        return source_;
    }

    void setSource(const QVariant& v);

    friend QDataStream& operator<<(QDataStream& s, const Screen& v);
    friend QDataStream& operator>>(QDataStream& s, Screen& v);

signals:
    void statusChanged(Status v);
    void sizeChanged(const QSize& v);
    void sourceChanged(const QVariant& v);

private slots:
    void onComponentStatusChanged(QQmlComponent::Status st);

private:
    QSize size_;
    QVariant source_;
    std::unique_ptr<QQmlComponent> component_;
    QQuickItem* root_item_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCREEN_H
