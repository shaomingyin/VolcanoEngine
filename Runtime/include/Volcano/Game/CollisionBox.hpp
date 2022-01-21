//
//
#ifndef VOLCANO_PHYSICS_COLLISIONBOX_HPP
#define VOLCANO_GAME_COLLISIONBOX_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/CollisionShape.hpp>

VOLCANO_GAME_BEGIN

class CollisionBox: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(qreal length READ length WRITE setLength NOTIFY lengthChanged)
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)

public:
    CollisionBox(QObject *parent = nullptr);
    CollisionBox(qreal length, qreal width, qreal height, QObject *parent = nullptr);
    ~CollisionBox(void) override;

public:
    qreal length(void) const;
    void setLength(qreal v);
    qreal width(void) const;
    void setWidth(qreal v);
    qreal height(void) const;
    void setHeight(qreal v);

signals:
    void lengthChanged(qreal v);
    void widthChanged(qreal v);
    void heightChanged(qreal v);

protected:
    void onTick(Duration elapsed) override;

private:
    void reload(void);

private:
    qreal m_length;
    qreal m_width;
    qreal m_height;
    Physics::Box *m_physicsBox;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COLLISIONBOX_HPP
