//
//
#ifndef VOLCANO_PHYSICS_BOXCOLLISIONSHAPE_H
#define VOLCANO_PHYSICS_BOXCOLLISIONSHAPE_H

#include <QVector3D>

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include <Volcano/Physics/CollisionShape.h>

VOLCANO_PHYSICS_BEGIN

class BoxCollisionShape: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(QVector3D size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(float convexRadius READ convexRadius WRITE setConvexRadius NOTIFY convexRadiusChanged)
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged)

public:
    explicit BoxCollisionShape(QObject* parent = nullptr);

public:
    QVector3D size() const noexcept;
    void setSize(const QVector3D& size);
    float convexRadius() const noexcept;
    void setConvexRadius(float radius);
    bool isValid() const noexcept override;

signals:
    void sizeChanged(const QVector3D& size);
    void convexRadiusChanged(float radius);
    void validChanged(bool valid);

private:
    const JPH::BoxShapeSettings& settings() const noexcept override;

private:
    JPH::BoxShapeSettings settings_ { JPH::Vec3::sReplicate(0.5f), 0.05f };
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_BOXCOLLISIONSHAPE_H
