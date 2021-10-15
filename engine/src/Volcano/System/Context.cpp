//
//
#include <Volcano/System/Context.hpp>

VOLCANO_SYSTEM_BEGIN

Context::Context(QObject *parent):
    QObject(parent)
{
}

Context::~Context(void)
{
}

void Context::setIODevice(QIODevice *p)
{
    m_stream.setDevice(p);
    m_stream.resetStatus();
}

QDataStream &Context::stream(void)
{
    return m_stream;
}

VOLCANO_SYSTEM_END
