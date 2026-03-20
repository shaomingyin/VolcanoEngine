//
//
#ifndef VOLCANO_WORLD_CAPSULECOLLISIONSHAPE_H
#define VOLCANO_WORLD_CAPSULECOLLISIONSHAPE_H

#include <memory>

#include <Volcano/World/Common.h>
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

class CapsuleCollisionShape: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    CapsuleCollisionShape(QObject* parent = nullptr);

public:
    float radius() const {
        return radius_;
    }


    float height() const {
        return height_;
    }

    void setRadius(float v);
    void setHeight(float v);
    void componentComplete() override;

signals:
    void radiusChanged(float v);
    void heightChanged(float v);

private:
    float radius_;
    float height_;
    std::unique_ptr<btCapsuleShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAPSULECOLLISIONSHAPE_H
