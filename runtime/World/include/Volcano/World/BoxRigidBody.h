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
        auto tmp = shape_->getImplicitShapeDimensions();
        return { tmp.x(), tmp.y(), tmp.z() };
    }

    Q_INVOKABLE void resize(float l, float w, float h) {
        resize({ l, w, h });
    }

    float length() const {
        return size().x();
    }

    void setLength(float v) {
        auto tmp = shape_->getImplicitShapeDimensions();
        resize(v, tmp.y(), tmp.z());
        emit lengthChanged(v);
    }

    float width() const {
        return size().y();
    }

    void setWidth(float v) {
        auto tmp = shape_->getImplicitShapeDimensions();
        resize(tmp.x(), v, tmp.z());
        emit widthChanged(v);
    }

    float height() const {
        return size().z();
    }

    void setHeight(float v) {
        auto tmp = shape_->getImplicitShapeDimensions();
        resize(tmp.x(), tmp.y(), v);
        emit heightChanged(v);
    }

    void resize(const QVector3D& v);

signals:
    void sizeChanged(const QVector3D& v);
    void lengthChanged(float v);
    void widthChanged(float v);
    void heightChanged(float v);

private:
    std::unique_ptr<btBoxShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BOXRIGIDBODY_H
