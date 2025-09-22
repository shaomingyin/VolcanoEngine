//
//
#ifndef VOLCANO_SYSTEM_SERVER_H
#define VOLCANO_SYSTEM_SERVER_H

#include <Volcano/System/Common.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class Server: public Base {
    Q_OBJECT

public:
    Server(World::Scene& scene, QObject* parent = nullptr);

public:
    void update(Duration elapsed) override;

private:
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SERVER_H
