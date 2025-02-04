//
//
#ifndef VOLCANO_WORLD_AMBIENTLIGHT_H
#define VOLCANO_WORLD_AMBIENTLIGHT_H

#include <QColor>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transform.h>
#include <Volcano/World/Object.h>

VOLCANO_WORLD_BEGIN

class AmbientLight: public Object {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(float strength READ strength WRITE setStrength NOTIFY strengthChanged FINAL)

public:
    AmbientLight(QObject* parent = nullptr);

public:
    const QColor& color() const {
		return color_;
	}

    void setColor(const QColor& v) {
        color_ = v;
        emit colorChanged(v);
    }

	float strength() const {
		return strength_;
	}

    void setStrength(float v) {
        if (!qFuzzyCompare(strength_, v)) {
            strength_ = v;
            emit strengthChanged(v);
        }
    }

signals:
    void colorChanged(const QColor& v);
    void strengthChanged(float v);

private:
    QColor color_;
	float strength_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_AMBIENTLIGHT_H
