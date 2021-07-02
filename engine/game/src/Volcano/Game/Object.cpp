//
//
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

Object::Object(QObject *parent):
    QObject(parent),
    m_isEnabled(true)
{
}

Object::~Object(void)
{
}

void Object::update(float elapsed)
{
    Q_UNUSED(elapsed);
}

bool Object::isEnabled(void) const
{
    return m_isEnabled;
}

void Object::setEnabled(bool v)
{
    if (m_isEnabled != v) {
        m_isEnabled = v;
        emit enabledChanged(v);
    }
}

VOLCANO_GAME_END
