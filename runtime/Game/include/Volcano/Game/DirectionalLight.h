//
//
#ifndef VOLCANO_GAME_DIRECTIONALLIGHT_H
#define VOLCANO_GAME_DIRECTIONALLIGHT_H

#include <QVector3D>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Light.h>

VOLCANO_GAME_BEGIN

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

    friend QDataStream& operator<<(QDataStream& s, const DirectionalLight& v);
    friend QDataStream& operator>>(QDataStream& s, DirectionalLight& v);

signals:
    void directionChanged(const QVector3D& v);

private:
    QVector3D direction_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DIRECTIONALLIGHT_H
