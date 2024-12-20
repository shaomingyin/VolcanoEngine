//
//
#ifndef VOLCANO_PHYSICS_TRIANGLEMESHRIGIDBODY_H
#define VOLCANO_PHYSICS_TRIANGLEMESHRIGIDBODY_H

#include <memory>

#include <QUrl>

#include <Volcano/Physics/Common.h>
#include <Volcano/Physics/RigidBody.h>

VOLCANO_PHYSICS_BEGIN

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

signals:
    void sourceChanged(const QUrl& v);

private:
    QUrl source_;
    std::unique_ptr<btTriangleMeshShape> shape_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_TRIANGLEMESHRIGIDBODY_H
