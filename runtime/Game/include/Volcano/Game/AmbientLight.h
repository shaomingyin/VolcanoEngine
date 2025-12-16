//
//
#ifndef VOLCANO_GAME_AMBIENTLIGHT_H
#define VOLCANO_GAME_AMBIENTLIGHT_H

#include <QColor>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Transform.h>
#include <Volcano/Game/Object.h>

VOLCANO_GAME_BEGIN

class AmbientLight: public Object {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(float strength READ strength WRITE setStrength NOTIFY strengthChanged FINAL)

public:
    AmbientLight(QObject* parent = nullptr);
    AmbientLight(Context& context, QObject* parent = nullptr);

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

    friend QDataStream& operator<<(QDataStream& s, const AmbientLight& v);
    friend QDataStream& operator>>(QDataStream& s, AmbientLight& v);

signals:
    void colorChanged(const QColor& v);
    void strengthChanged(float v);

private:
    QColor color_;
	float strength_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_AMBIENTLIGHT_H
