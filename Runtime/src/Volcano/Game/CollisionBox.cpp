//
//
#include <Volcano/Game/CollisionBox.hpp>

VOLCANO_GAME_BEGIN

CollisionBox::CollisionBox(QObject *parent):
    CollisionShape(parent),
    m_length(1.0f),
    m_width(1.0f),
    m_height(1.0f),
    m_physicsBox(nullptr)
{
}

CollisionBox::CollisionBox(qreal length, qreal width, qreal height, QObject *parent):
    CollisionShape(parent),
    m_length(length),
    m_width(width),
    m_height(height),
    m_physicsBox(nullptr)
{
}

CollisionBox::~CollisionBox(void)
{
    if (m_physicsBox != nullptr) {
        delete m_physicsBox;
        m_physicsBox = nullptr;
    }
}

qreal CollisionBox::length(void) const
{
    return m_length;
}

void CollisionBox::setLength(qreal v)
{
    if (!qFuzzyCompare(m_length, v)) {
        m_length = v;
        reload();
        emit lengthChanged(v);
    }
}

qreal CollisionBox::width(void) const
{
    return m_width;
}

void CollisionBox::setWidth(qreal v)
{
    if (!qFuzzyCompare(m_width, v)) {
        m_width = v;
        reload();
        emit widthChanged(v);
    }
}

qreal CollisionBox::height(void) const
{
    return m_height;
}

void CollisionBox::setHeight(qreal v)
{
    if (!qFuzzyCompare(m_height, v)) {
        m_height = v;
        reload();
        emit heightChanged(v);
    }
}

void CollisionBox::onTick(Duration elapsed)
{
    if (m_physicsBox == nullptr)
        reload();
}

void CollisionBox::reload(void)
{
    if (m_physicsBox != nullptr) {
        delete m_physicsBox;
        m_physicsBox = nullptr;
    }

    m_physicsBox = physicsService()->createBox(m_length, m_width, m_height);
}

VOLCANO_GAME_END
