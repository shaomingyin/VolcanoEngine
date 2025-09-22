//
//
#ifndef VOLCANO_WORLD_DIRECTIONALLIGHT_H
#define VOLCANO_WORLD_DIRECTIONALLIGHT_H

#include <QVector3D>

#include <Volcano/World/Common.h>
#include <Volcano/World/Light.h>

VOLCANO_WORLD_BEGIN

class DirectionalLight: public Light {
    Q_OBJECT
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged FINAL)

public:
    DirectionalLight(QObject* parent = nullptr);

public:
    const QVector3D& direction() const {
		return direction_;
	}

    void setDirection(const QVector3D& v) {
        if (!qFuzzyCompare(direction_, v)) {
            direction_ = v;
            emit directionChanged(v);
        }
    }

signals:
    void directionChanged(const QVector3D& v);

private:
    QVector3D direction_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_DIRECTIONALLIGHT_H
