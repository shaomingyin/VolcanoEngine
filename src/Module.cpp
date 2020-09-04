//
//
#include <Volcano/Module.hpp>

VOLCANO_BEGIN

Module::Module(QObject *parent):
    QLibrary(parent)
{
}

Module::~Module(void)
{
}

VOLCANO_END
