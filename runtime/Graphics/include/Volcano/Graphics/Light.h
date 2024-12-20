//
//
#ifndef VOLCANO_GRAHPICS_LIGHT_H
#define VOLCANO_GRAHPICS_LIGHT_H

#include <QColor>

#include <Volcano/World/Transformable.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Light: public World::Transformable {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(float strength READ strength WRITE setStrength NOTIFY strengthChanged FINAL)

public:
    Light(QObject* parent = nullptr);

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
        float tmp = std::clamp(v, 0.0f, 1.0f);
        if (!qFuzzyCompare(strength_, tmp)) {
            strength_ = tmp;
            emit strengthChanged(tmp);
        }
	}

signals:
    void colorChanged(const QColor& v);
    void strengthChanged(float v);

private:
    QColor color_;
	float strength_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAHPICS_LIGHT_H
