//
//
#include <Volcano/Game/LoadableComponent.hpp>

VOLCANO_GAME_BEGIN

LoadableComponent::LoadableComponent(QObject *parent):
    Component(parent)
{
}

LoadableComponent::~LoadableComponent(void)
{
}

QUrl LoadableComponent::source(void) const
{
    return m_source;
}

void LoadableComponent::setSource(QUrl v)
{
    if (m_source != v) {
        m_source = v;
        emit sourceChanged(v);
    }
}

VOLCANO_GAME_END
