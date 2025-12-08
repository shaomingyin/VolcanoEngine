//
//
#ifndef VOLCANO_GAME_TRIANGLEMESHRIGIDBODY_H
#define VOLCANO_GAME_TRIANGLEMESHRIGIDBODY_H

#include <memory>

#include <QUrl>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/RigidBody.h>

VOLCANO_GAME_BEGIN

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

VOLCANO_GAME_END

#endif // VOLCANO_GAME_TRIANGLEMESHRIGIDBODY_H
