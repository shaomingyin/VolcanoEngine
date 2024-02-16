//
//
#ifndef VOLCANO_ACTOR_HPP
#define VOLCANO_ACTOR_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Transform.hpp>
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

class Actor: public Object {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scaling WRITE setScale NOTIFY scalingChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    Actor(QObject* parent = nullptr);
    Actor(Context& context, QObject* parent = nullptr);

public:
    Transform& transform() {
        return transform_;
    }

    const Transform& transform() const {
        return transform_;
    }

    const QVector3D& position() const {
        return transform_.translation();
    }

    void setPosition(const QVector3D& v) {
        if (!qFuzzyCompare(transform_.translation(), v)) {
            transform_.setTranslation(v);
            emit positionChanged(v);
        }
    }

    void move(const QVector3D& v) {
        transform_.translate(v);
        emit positionChanged(transform_.translation());
    }

    const QVector3D& scaling() const {
        return transform_.scaling();
    }

    void setScale(const QVector3D& v) {
        if (!qFuzzyCompare(transform_.scaling(), v)) {
            transform_.setScaling(v);
            emit scalingChanged(v);
        }
    }

    void scale(const QVector3D& v) {
        transform_.scale(v);
        emit scalingChanged(transform_.scaling());
    }

    const QQuaternion& rotation() const {
        return transform_.rotation();
    }

    void setRotation(const QQuaternion& v) {
        if (!qFuzzyCompare(transform_.rotation(), v)) {
            transform_.setRotation(v);
            emit rotationChanged(v);
        }
    }

    void rotate(const QQuaternion& v) {
        transform_.rotate(v);
        emit rotationChanged(transform_.rotation());
    }

signals:
    void positionChanged(const QVector3D& v);
    void scalingChanged(const QVector3D& v);
    void rotationChanged(const QQuaternion& v);

private:
    Transform transform_;
};

VOLCANO_END

#endif // VOLCANO_ACTOR_HPP
