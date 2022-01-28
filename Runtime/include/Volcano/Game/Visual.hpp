//
//
#ifndef VOLCANO_GAME_VISUAL_HPP
#define VOLCANO_GAME_VISUAL_HPP

#include <Volcano/Shape.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Material.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Visual: public Component {
    Q_OBJECT
    Q_PROPERTY(Shape *shape READ shape WRITE setShape NOTIFY shapeChanged)

public:
    Visual(QObject *parent = nullptr);
    ~Visual(void) override;

public:
    Shape *shape(void);
    void setShape(Shape *p);

signals:
    void shapeChanged(Shape *p);

private:
    Shape *m_shape;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_VISUAL_HPP
