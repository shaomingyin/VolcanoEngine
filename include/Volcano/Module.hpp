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

#define VOLCANO_MODULE_IID "Volcano.Module/0.1"

Q_DECLARE_INTERFACE(Volcano::Module, VOLCANO_MODULE_IID)

#endif // VOLCANO_MODULE_HPP
