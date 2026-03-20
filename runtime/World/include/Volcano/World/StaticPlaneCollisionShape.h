//
//
#ifndef VOLCANO_WORLD_STATICPLANECOLLISIONSHAPE_H
#define VOLCANO_WORLD_STATICPLANECOLLISIONSHAPE_H

#include <memory>

#include <QVector3D>

#include <Volcano/World/Common.h>
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

class StaticPlaneCollisionShape: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(QVector3D normal READ normal WRITE setNormal NOTIFY normalChanged FINAL)
    Q_PROPERTY(float constant READ constant WRITE setConstant NOTIFY constantChanged FINAL)

public:
    StaticPlaneCollisionShape(QObject* parent = nullptr);

public:
    QVector3D normal() const {
        return normal_;
    }

    float constant() const {
        return constant_;
    }

    void setNormal(const QVector3D& v);
    void setConstant(float v);
    void componentComplete() override;

signals:
    void normalChanged(const QVector3D& v);
    void constantChanged(float v);

private:
    QVector3D normal_;
    float constant_;
    std::unique_ptr<btStaticPlaneShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_STATICPLANECOLLISIONSHAPE_H
