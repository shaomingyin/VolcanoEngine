//
//
#ifndef VOLCANO_GAME_ACTOR_HPP
#define VOLCANO_GAME_ACTOR_HPP

#include <QVector3D>
#include <QQuaternion>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Actor: public Object {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scaling WRITE setScale NOTIFY scalingChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    Actor(QObject* parent = nullptr);
    Actor(Context& context, QObject* parent = nullptr);

public:
    Eigen::Affine3f& transform() {
        return affine_;
    }

    const Eigen::Affine3f& transform() const {
        return affine_;
    }

    QVector3D position() const {
        auto translation = affine_.translation();
        return QVector3D(translation.x(), translation.y(), translation.z());
    }

    void setPosition(const QVector3D& v) {
        auto translation = Eigen::Vector3f(v.x(), v.y(), v.z());
        if (affine_.translation() != translation) {
            affine_.translate(translation);
            emit positionChanged(v);
        }
    }

    void move(const QVector3D& v) {
        affine_.translate(Eigen::Vector3f(v.x(), v.y(), v.z()));
        auto translation = affine_.translation();
        emit positionChanged(QVector3D(translation.x(), translation.y(), translation.z()));
    }

    const QVector3D& scaling() const {
        return scaling_;
    }

    void setScale(const QVector3D& v) {
        if (!qFuzzyCompare(scaling_, v)) {
            affine_.scale(Eigen::Vector3f(v.x(), v.y(), v.z()));
            scaling_ = v;
            emit scalingChanged(scaling_);
        }
    }

    void scale(const QVector3D& v) {
        affine_.scale(Eigen::Vector3f(v.x(), v.y(), v.z()));
        scaling_ *= v;
        emit scalingChanged(scaling_);
    }

    QQuaternion rotation() const {
        Eigen::Quaternionf rotation(affine_.rotation());
        return QQuaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w());
    }

    void setRotation(const QQuaternion& v) {
        Eigen::Quaternionf target(v.scalar(), v.x(), v.y(), v.z());
        Eigen::Quaternionf rotation(affine_.rotation());
        if (rotation != target) {
            affine_.rotate(target);
            emit rotationChanged(v);
        }
    }

    void rotate(const QQuaternion& v) {
        Eigen::Quaternionf target(v.scalar(), v.x(), v.y(), v.z());
        affine_.rotate(target);
        emit rotationChanged(QQuaternion(target.w(), target.x(), target.y(), target.z()));
    }

signals:
    void positionChanged(const QVector3D& v);
    void scalingChanged(const QVector3D& v);
    void rotationChanged(const QQuaternion& v);

private:
    Eigen::Affine3f affine_;
    QVector3D scaling_; // cache
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ACTOR_HPP
