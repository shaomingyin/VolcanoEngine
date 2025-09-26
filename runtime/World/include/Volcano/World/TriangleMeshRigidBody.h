//
//
#ifndef VOLCANO_WORLD_TRIANGLEMESHRIGIDBODY_H
#define VOLCANO_WORLD_TRIANGLEMESHRIGIDBODY_H

#include <memory>

#include <QUrl>

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

class TriangleMeshRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)

public:
    TriangleMeshRigidBody(QObject* parent = nullptr);

public:
    const QUrl& source() const {
        return source_;
    }

    void setSource(const QUrl& v);
    void componentComplete() override;

    friend QDataStream& operator<<(QDataStream& s, const TriangleMeshRigidBody& v);
    friend QDataStream& operator>>(QDataStream& s, TriangleMeshRigidBody& v);

signals:
    void sourceChanged(const QUrl& v);

private:
    QUrl source_;
    std::unique_ptr<btTriangleMeshShape> shape_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TRIANGLEMESHRIGIDBODY_H
