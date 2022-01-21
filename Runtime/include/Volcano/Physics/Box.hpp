//
//
#ifndef VOLCANO_PHYSICS_BOX_HPP
#define VOLCANO_PHYSICS_BOX_HPP

#include <Volcano/Physics/Common.hpp>
#include <Volcano/Physics/Shape.hpp>

VOLCANO_PHYSICS_BEGIN

class Box: public Shape {
public:
    Box(void);
    Box(qreal length, qreal width, qreal height);
    ~Box(void) override;

public:
    qreal length(void) const;
    void setLength(qreal v);
    qreal width(void) const;
    void setWidth(qreal v);
    qreal height(void) const;
    void setHeight(qreal v);

private:
    qreal m_length;
    qreal m_width;
    qreal m_height;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_BOX_HPP
