//
//
#include <Volcano/VM/Component.hpp>

VOLCANO_VM_BEGIN

Component::Component(void):
    m_flags(0)
{
}

Component::~Component(void)
{
    if (isEnabled())
        disable();
}

void Component::enable(void)
{
    if (!(m_flags & FLAG_ENABLED)) {
        m_flags |= FLAG_ENABLED;
        onEnabled();
    }
}

void Component::disable(void)
{
    if (m_flags & FLAG_ENABLED) {
        m_flags &= ~FLAG_ENABLED;
        onDisabled();
    }
}

void Component::onUpdate(float elapsed)
{
    VOLCANO_UNUSED(elapsed);
}

bool Component::onEvent(const SDL_Event &evt)
{
    VOLCANO_UNUSED(evt);

    return false;
}

void Component::addToView(Graphics::View &view)
{
    VOLCANO_UNUSED(view);
}

void Component::onNameChanged(const std::string &name)
{
    VOLCANO_UNUSED(name);
}

void Component::onEnabled(void)
{
}

void Component::onDisabled(void)
{
}

VOLCANO_VM_END
