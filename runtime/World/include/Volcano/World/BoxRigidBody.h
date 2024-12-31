//
//
#ifndef VOLCANO_WORLD_BOXRIGIDBODY_H
#define VOLCANO_WORLD_BOXRIGIDBODY_H

#include <memory>

#include <QVector3D>

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class BoxRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(QVector3D size READ size WRITE resize NOTIFY sizeChanged FINAL)
    Q_PROPERTY(float length READ length WRITE setLength NOTIFY lengthChanged FINAL)
    Q_PROPERTY(float width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    BoxRigidBody(QObject* parent = nullptr);

public:
    QVector3D size() const {
        return size_;
    }

    void resize(const QVector3D& v) {
        if (!qFuzzyCompare(size_, v)) {
            resize(v.x(), v.y(), v.z());
        }
    }

    float length() const {
        return size_.x();
    }

    void setLength(float v) {
        if (!qFuzzyCompare(size_.x(), v)) {
            forceResize({ v, size_.y(), size_.z() });
            emit lengthChanged(v);
        }
    }

    float width() const {
        return size_.y();
    }

    void setWidth(float v) {
        if (!qFuzzyCompare(size_.z(), v)) {
            forceResize({ size_.x(), size_.y(), v });
            emit widthChanged(v);
        }
    }

    float height() const {
        return size_.z();
    }

    void setHeight(float v) {
        if (!qFuzzyCompare(size_.y(), v)) {
            forceResize({ size_.x(), v, size_.z() });
            emit heightChanged(v);
        }
    }

    Q_INVOKABLE void resize(float x, float y, float z);

signals:
    void sizeChanged(const QVector3D& v);
    void lengthChanged(float v);
    void widthChanged(float v);
    void heightChanged(float v);

private:
    QVector3D size_;
    std::unique_ptr<btBoxShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BOXRIGIDBODY_H
