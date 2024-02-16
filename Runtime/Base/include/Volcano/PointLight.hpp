//
//
#ifndef VOLCANO_POINTLIGHT_HPP
#define VOLCANO_POINTLIGHT_HPP

#include <QVector3D>

#include <Volcano/Common.hpp>
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

class PointLight: public Light {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)

public:
    PointLight(QObject* parent = nullptr);
    PointLight(Context& context, QObject* parent = nullptr);

public:
    const QVector3D& position(void) const;
    void setPosition(const QVector3D& v);

signals:
    void positionChanged(const QVector3D& v);

private:
    QVector3D position_;
};

VOLCANO_END

#endif // VOLCANO_POINTLIGHT_HPP
