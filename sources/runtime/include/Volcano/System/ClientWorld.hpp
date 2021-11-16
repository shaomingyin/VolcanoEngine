//
//
#ifndef VOLCANO_SYSTEM_CLIENTWORLD_HPP
#define VOLCANO_SYSTEM_CLIENTWORLD_HPP

#include <Volcano/Graphics/World.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class ClientWorld: public Graphics::World {
    Q_OBJECT

public:
    ClientWorld(QObject *parent = nullptr);
    ~ClientWorld(void) override;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CLIENTWORLD_HPP
