//
//
#include <Volcano/Updater.hpp>

VOLCANO_BEGIN

Updater::Updater(Callback cb, Object *parent):
    Object(parent),
    m_cb(cb)
{
    VOLCANO_ASSERT(m_cb);
}

Updater::~Updater(void)
{
}

Duration Updater::onUpdate(void)
{
    m_cb();

    return Duration::max();
}

VOLCANO_END
