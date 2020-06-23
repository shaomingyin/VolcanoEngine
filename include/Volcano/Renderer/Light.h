//
//
#ifndef VOLCANO_RENDERER_LIGHT_H
#define VOLCANO_RENDERER_LIGHT_H

#include <Volcano/Object.h>
#include <Volcano/Renderer/Common.h>

VOLCANO_RENDERER_BEGIN

class VOLCANO_API Light: public Object
{
    Q_OBJECT

public:
    Light(QObject *parent = nullptr);
    virtual ~Light(void);

private:
    // mesh resource
};

VOLCANO_RENDERER_END

#endif // VOLCANO_RENDERER_LIGHT_H
