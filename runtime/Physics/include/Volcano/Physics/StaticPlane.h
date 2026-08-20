//
//
#ifndef VOLCANO_WORLD_STATICPLANE_H
#define VOLCANO_WORLD_STATICPLANE_H

#include <QVector3D>

#include <Volcano/World/Common.h>
#include <Volcano/World/Shape.h>

VOLCANO_WORLD_BEGIN

class StaticPlane: public Shape {
    Q_OBJECT
    Q_PROPERTY(QVector3D normal READ normal WRITE setNormal NOTIFY normalChanged)
    Q_PROPERTY(float constant READ constant WRITE setConstant NOTIFY constantChanged)

public:
    StaticPlane(QObject* parent = nullptr);

public:
    QVector3D normal() const noexcept;
    float constant() const noexcept;
    void setNormal(const QVector3D& v) noexcept;
    void setConstant(float v) noexcept;

signals:
    void normalChanged(const QVector3D& v);
    void constantChanged(float v);

private:
    QVector3D normal_;
    float constant_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_STATICPLANE_H
