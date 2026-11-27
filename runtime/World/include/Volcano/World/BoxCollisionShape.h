//
//
#ifndef VOLCANO_WORLD_BOXCOLLISIONSHAPE_H
#define VOLCANO_WORLD_BOXCOLLISIONSHAPE_H

#include <QVector3D>

#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

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
    QVector3D size_ { 1.0f, 1.0f, 1.0f };
    float convexRadius_ = 0.05f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_BOXCOLLISIONSHAPE_H
