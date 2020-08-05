//
//
#include <Volcano/Resource.hpp>

VOLCANO_BEGIN

Resource::Cache Resource::c_cache(512);

Resource::Resource(Node *parent):
    Node(parent),
    m_state(StateFree),
    m_progress(0.0f)
{
}

Resource::~Resource(void)
{
}

void Resource::setSource(const QUrl &v)
{
    if (m_source == v)
        return;

    m_source = v;
    sourceChanged();

    setState(StateDirty);
    setProgress(0.0f);
}

VOLCANO_END
