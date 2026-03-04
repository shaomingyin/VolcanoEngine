//
//
#ifndef VOLCANO_GAME_CYLINDERRIGIDBODY_H
#define VOLCANO_GAME_CYLINDERRIGIDBODY_H

#include <memory>

#include <QVector3D>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/RigidBody.h>

VOLCANO_GAME_BEGIN

class CylinderRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(QVector3D size READ size WRITE resize NOTIFY sizeChanged FINAL)

public:
    CylinderRigidBody(QObject* parent = nullptr);

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

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CYLINDERRIGIDBODY_H
