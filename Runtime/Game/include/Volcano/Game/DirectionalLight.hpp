//
//
#ifndef VOLCANO_GAME_DIRECTIONALIGHT_HPP
#define VOLCANO_GAME_DIRECTIONALIGHT_HPP

#include <QVector3D>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API DirectionalLight: public Light {
    Q_OBJECT
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)

public:
    DirectionalLight(QObject* parent = nullptr);
    DirectionalLight(Context& context, QObject* parent = nullptr);

public:
    const QVector3D& direction(void) const;
    void setDirection(const QVector3D& v);

signals:
    void directionChanged(const QVector3D& v);

private:
    QVector3D direction_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DIRECTIONALIGHT_HPP
