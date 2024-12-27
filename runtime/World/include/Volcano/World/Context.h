//
//
#ifndef VOLCANO_WORLD_CONTEXT_H
#define VOLCANO_WORLD_CONTEXT_H

#include <QObject>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Context: public QObject {
    Q_OBJECT

public:
    Context(QObject* parent = nullptr);

public:
    //virtual
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CONTEXT_H
