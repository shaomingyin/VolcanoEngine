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

public:
    SpotLight(QObject *parent = nullptr);
    ~SpotLight(void) override;

public:
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);

signals:
    void positionChanged(const QVector3D &v);

private:
    QVector3D m_position;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPOTLIGHT_HPP
