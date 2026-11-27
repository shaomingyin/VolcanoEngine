//
//
#ifndef VOLCANO_WORLD_RIGIDBODY_H
#define VOLCANO_WORLD_RIGIDBODY_H

#include <QPointer>

#include <Volcano/World/Common.h>
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

class RigidBody: public QObject {
    Q_OBJECT
    Q_PROPERTY(CollisionShape* shape READ shape WRITE setShape NOTIFY shapeChanged)
    Q_PROPERTY(MotionType motionType READ motionType WRITE setMotionType NOTIFY motionTypeChanged)
    Q_PROPERTY(MotionQuality motionQuality READ motionQuality WRITE setMotionQuality NOTIFY motionQualityChanged)
    Q_PROPERTY(uint objectLayer READ objectLayer WRITE setObjectLayer NOTIFY objectLayerChanged)
    Q_PROPERTY(bool sensor READ isSensor WRITE setSensor NOTIFY sensorChanged)
    Q_PROPERTY(float friction READ friction WRITE setFriction NOTIFY frictionChanged)
    Q_PROPERTY(float restitution READ restitution WRITE setRestitution NOTIFY restitutionChanged)
    Q_PROPERTY(float gravityFactor READ gravityFactor WRITE setGravityFactor NOTIFY gravityFactorChanged)

public:
    enum class MotionType {
        Static,
        Kinematic,
        Dynamic
    };
    Q_ENUM(MotionType)

    enum class MotionQuality {
        Discrete,
        LinearCast
    };
    Q_ENUM(MotionQuality)

public:
    explicit RigidBody(QObject* parent = nullptr);

public:
    CollisionShape* shape() const noexcept;
    void setShape(CollisionShape* shape);
    MotionType motionType() const noexcept;
    void setMotionType(MotionType type);
    MotionQuality motionQuality() const noexcept;
    void setMotionQuality(MotionQuality quality);
    uint objectLayer() const noexcept;
    void setObjectLayer(uint layer);
    bool isSensor() const noexcept;
    void setSensor(bool sensor);
    float friction() const noexcept;
    void setFriction(float friction);
    float restitution() const noexcept;
    void setRestitution(float restitution);
    float gravityFactor() const noexcept;
    void setGravityFactor(float factor);

signals:
    void shapeChanged(CollisionShape* shape);
    void motionTypeChanged(MotionType type);
    void motionQualityChanged(MotionQuality quality);
    void objectLayerChanged(uint layer);
    void sensorChanged(bool sensor);
    void frictionChanged(float friction);
    void restitutionChanged(float restitution);
    void gravityFactorChanged(float factor);
    void settingsChanged();

private:
    QPointer<CollisionShape> shape_;
    MotionType motionType_ = MotionType::Dynamic;
    MotionQuality motionQuality_ = MotionQuality::Discrete;
    uint objectLayer_ = 1;
    bool sensor_ = false;
    float friction_ = 0.2f;
    float restitution_ = 0.0f;
    float gravityFactor_ = 1.0f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_RIGIDBODY_H
