//
//
#include <Volcano/Context.hpp>

VOLCANO_BEGIN

Context::Context(QObject *parent):
    QObject(parent)
{
}

Context::~Context(void)
{
}

bool Context::init(void)
{
    return true;
}

VOLCANO_END
