//
//
#ifndef VOLCANO_MODULE_HPP
#define VOLCANO_MODULE_HPP

#include <QString>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API Module
{
public:
    virtual ~Module(void);

public:
    virtual bool init(void) = 0;
    virtual void shutdown(void) = 0;
    virtual void frame(float elapsed) = 0;
    virtual const QString &name(void) const = 0;
    virtual int version(void) const = 0;
};

VOLCANO_END

#endif // VOLCANO_MODULE_HPP
