//
//
#ifndef VOLCANO_WORLD_CYLINDERCOLLISIONSHAPE_H
#define VOLCANO_WORLD_CYLINDERCOLLISIONSHAPE_H

#include <memory>

#include <QVector3D>

#include <Volcano/World/Common.h>
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

class CylinderCollisionShape: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(QVector3D size READ size WRITE resize NOTIFY sizeChanged FINAL)

public:
    CylinderCollisionShape(QObject* parent = nullptr);

public:
    const QVector3D& size() const {
        return size_;
    }

    void resize(const QVector3D& v);
    void componentComplete() override;

signals:
    void sizeChanged(const QVector3D& v);

private:
    QVector3D size_;
    std::unique_ptr<btCylinderShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CYLINDERCOLLISIONSHAPE_H
