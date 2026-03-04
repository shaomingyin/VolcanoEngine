//
//
#ifndef VOLCANO_GAME_BOXRIGIDBODY_H
#define VOLCANO_GAME_BOXRIGIDBODY_H

#include <memory>

#include <QVector3D>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/RigidBody.h>

VOLCANO_GAME_BEGIN

class BoxRigidBody: public RigidBody {
    Q_OBJECT
    Q_PROPERTY(float length READ length WRITE setLength NOTIFY lengthChanged FINAL)
    Q_PROPERTY(float width READ width WRITE setWidth NOTIFY widthChanged FINAL)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged FINAL)

public:
    BoxRigidBody(QObject* parent = nullptr);

public:
    QVector3D size() const {
        return size_;
    }

    float length() const {
        return size_.x();
    }

    void setLength(float v) {
        if (!qFuzzyCompare(size_.x(), v)) {
            size_.setX(v);
            emit lengthChanged(v);
        }
    }

    float width() const {
        return size_.y();
    }

    float height() const {
        return size_.z();
    }

    void setWidth(float v);
    void setHeight(float v);
    void componentComplete() override;

signals:
    void lengthChanged(float v);
    void widthChanged(float v);
    void heightChanged(float v);

private:
    QVector3D size_;
    std::unique_ptr<btBoxShape> shape_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_BOXRIGIDBODY_H
