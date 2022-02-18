//
//
#include <Volcano/Game/Visual.hpp>

VOLCANO_GAME_BEGIN

Visual::Visual(QObject *parent):
    Component(parent),
    m_shape(nullptr)
{
}

Visual::Visual(Context *context, QObject *parent):
    Component(context, parent),
    m_shape(nullptr)
{
}

Visual::~Visual(void)
{
}

Shape *Visual::shape(void)
{
    return m_shape;
}

void Visual::setShape(Shape *p)
{
    if (m_shape == p)
        return;

    // TODO physics service.

    m_shape = p;
    emit shapeChanged(p);
}

VOLCANO_GAME_END
