//
//
#ifndef VOLCANO_GAME_ACTOR_HPP
#define VOLCANO_GAME_ACTOR_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Actor: public Object {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    Actor(QObject* parent = nullptr);
    ~Actor(void) override;

public:
    const QVector3D& position() const;
    void setPosition(const QVector3D& v);
    const QVector3D& scale() const;
    void setScale(const QVector3D& v);
    const QQuaternion& rotation() const;
    void setRotation(const QQuaternion& v);

signals:
    void positionChanged(const QVector3D& v);
    void scaleChanged(const QVector3D& v);
    void rotationChanged(const QQuaternion& v);

private:
    QVector3D position_;
    QVector3D scale_;
    QQuaternion rotation_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ACTOR_HPP
