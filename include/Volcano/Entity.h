//
//
#ifndef VOLCANO_ENTITY_H
#define VOLCANO_ENTITY_H

#include <Volcano/Common.h>
#include <Volcano/Node.h>

VOLCANO_BEGIN

class VOLCANO_API Entity: public Node
{
    Q_OBJECT

public:
    Entity(QObject *parent = nullptr);
    virtual ~Entity(void);

private:
    // mesh resource
};

VOLCANO_END

#endif // VOLCANO_ENTITY_H
