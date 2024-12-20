//
//
#ifndef VOLCANO_GRAPHICS_DIRECTIONALLIGHT_H
#define VOLCANO_GRAPHICS_DIRECTIONALLIGHT_H

#include <QVector3D>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Light.h>

VOLCANO_GRAPHICS_BEGIN

class DirectionalLight: public Light {
    Q_OBJECT
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged FINAL)

public:
    DirectionalLight(QObject* parent = nullptr);

public:
    const QVector3D& direction() const {
		return direction_;
	}

    void setDirection(QVector3D v) {
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

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DIRECTIONALLIGHT_H
