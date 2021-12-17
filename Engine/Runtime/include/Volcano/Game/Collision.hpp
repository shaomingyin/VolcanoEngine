//
//
#ifndef VOLCANO_GAME_COLLISION_HPP
#define VOLCANO_GAME_COLLISION_HPP

#include <Volcano/Shape.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Collision: public Component {
    Q_OBJECT
    Q_PROPERTY(Shape *shape READ shape WRITE setShape NOTIFY shapeChanged)

public:
    Collision(QObject *parent = nullptr);
    ~Collision(void) override;

public:
    Shape *shape(void);
    void setShape(Shape *p);

signals:
    void shapeChanged(Shape *p);

private:
    Shape *m_shape;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COLLISION_HPP
