//
//
#ifndef VOLCANO_GAME_DIRECTIONALLIGHT_HPP
#define VOLCANO_GAME_DIRECTIONALLIGHT_HPP

#include <QVector3D>

#include <Volcano/Common.hpp>
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

class DirectionalLight: public Light {
    Q_OBJECT
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    DirectionalLight(QObject *parent = nullptr);
    ~DirectionalLight(void) override;

public:
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &v);

signals:
    void directionChanged(const QVector3D &v);

private:
    QVector3D m_direction;
};

VOLCANO_END

#endif // VOLCANO_DIRECTIONALLIGHT_HPP
