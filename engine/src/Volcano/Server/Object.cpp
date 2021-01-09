//
//
#include <Volcano/Server/Object.hpp>

VOLCANO_SERVER_BEGIN

Object::Object(void):
    m_flags(0)
{
}

Object::~Object(void)
{
    if (isEnabled())
        disable();
}

void Object::enable(void)
{
    if (!(m_flags & FLAG_ENABLED)) {
        m_flags |= FLAG_ENABLED;
        onEnabled();
    }
}

void Object::disable(void)
{
    if (m_flags & FLAG_ENABLED) {
        m_flags &= ~FLAG_ENABLED;
        onDisabled();
    }
}

void Object::onUpdate(float elapsed)
{
    VOLCANO_UNUSED(elapsed);
}

void Object::onNameChanged(const std::string &name)
{
    VOLCANO_UNUSED(name);
}

void Object::onEnabled(void)
{
}

void Object::onDisabled(void)
{
}

VOLCANO_SERVER_END
