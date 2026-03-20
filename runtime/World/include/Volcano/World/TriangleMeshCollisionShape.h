//
//
#ifndef VOLCANO_WORLD_TRIANGLEMESHCOLLISIONSHAPE_H
#define VOLCANO_WORLD_TRIANGLEMESHCOLLISIONSHAPE_H

#include <memory>

#include <QUrl>

#include <Volcano/World/Common.h>
#include <Volcano/World/CollisionShape.h>

VOLCANO_WORLD_BEGIN

class TriangleMeshCollisionShape: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)

public:
    TriangleMeshCollisionShape(QObject* parent = nullptr);

public:
    const QUrl& source() const {
        return source_;
    }

    void setSource(const QUrl& v);
    void componentComplete() override;

signals:
    void sourceChanged(const QUrl& v);

private:
    QUrl source_;
    std::unique_ptr<btTriangleMeshShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRIANGLEMESHCOLLISIONSHAPE_H
