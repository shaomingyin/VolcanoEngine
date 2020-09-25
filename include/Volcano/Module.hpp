//
//
#ifndef VOLCANO_MODULE_HPP
#define VOLCANO_MODULE_HPP

#include <Volcano/Common.hpp>


VOLCANO_BEGIN

struct VOLCANO_API Module
{
    virtual bool init(void) = 0;
    virtual void shutdown(void) = 0;
};

VOLCANO_END

#define VOLCANO_MODULE_IID "Volcano.Module/" #VOLCANO_VERSION_MAJOR "." #VOLCANO_VERSION_MINOR

Q_DECLARE_INTERFACE(Volcano::Module, VOLCANO_MODULE_IID)

#endif // VOLCANO_MODULE_HPP
