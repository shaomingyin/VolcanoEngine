//
//
#include <Volcano/Resource.hpp>

VOLCANO_BEGIN

Resource::Cache Resource::c_cache(32);

Resource::Resource(QObject *parent):
    Component(parent),
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

    setState(StateDirty);
    setProgress(0.0f);
    m_source = v;
    sourceChanged(v);
}

VOLCANO_END
