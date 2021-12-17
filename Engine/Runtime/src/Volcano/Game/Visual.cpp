//
//
#include <Volcano/Game/Visual.hpp>

VOLCANO_GAME_BEGIN

Visual::Visual(QObject *parent):
    Component(parent),
    m_offset(0.0f, 0.0f, 0.0f),
    m_scale(1.0f, 1.0f, 1.0f),
    m_shape(nullptr),
    m_material(nullptr)
{
}

Visual::~Visual(void)
{
}

const QVector3D &Visual::offset(void) const
{
    return m_offset;
}

void Visual::setOffset(const QVector3D &v)
{
    m_offset = v;
    emit offsetChanged(v);
}

const QVector3D &Visual::scale(void) const
{
    return m_scale;
}

void Visual::setScale(const QVector3D &v)
{
    m_scale = v;
    emit scaleChanged(v);
}

Rotation *Visual::rotation(void)
{
    return &m_rotation;
}

Shape *Visual::shape(void)
{
    return m_shape;
}

void Visual::setShape(Shape *p)
{
    if (m_shape != p) {
        m_shape = p;
        emit shapeChanged(p);
    }
}

Material *Visual::material(void)
{
    return m_material;
}

void Visual::setMaterial(Material *p)
{
    if (m_material != p) {
        m_material = p;
        emit materialChanged(p);
    }
}

VOLCANO_GAME_END
