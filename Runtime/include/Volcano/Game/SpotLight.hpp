//
//
#ifndef VOLCANO_GAME_SPOTLIGHT_HPP
#define VOLCANO_GAME_SPOTLIGHT_HPP

#include <QVector3D>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/DirectionalLight.hpp>

VOLCANO_GAME_BEGIN

class SpotLight: public DirectionalLight {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qreal distance READ distance WRITE setDistance NOTIFY distanceChanged)

public:
    SpotLight(QObject *parent = nullptr);
    ~SpotLight(void) override;

public:
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    qreal distance(void) const;
    void setDistance(qreal v);

signals:
    void positionChanged(const QVector3D &v);
    void distanceChanged(qreal v);

private:
    QVector3D m_position;
    qreal m_distance;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPOTLIGHT_HPP
