//
//
#ifndef VOLCANO_LIGHT_H
#define VOLCANO_LIGHT_H

#include <Volcano/Node.h>
#include <Volcano/Common.h>

VOLCANO_BEGIN

class VOLCANO_API Light: public Node
{
    Q_OBJECT

public:
    Light(QObject *parent = nullptr);
    virtual ~Light(void);

private:
    // mesh resource
};

VOLCANO_END

#endif // VOLCANO_LIGHT_H
