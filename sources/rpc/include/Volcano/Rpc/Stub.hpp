//
//
#ifndef VOLCANO_RPC_SHUB_HPP
#define VOLCANO_RPC_STUB_HPP

#include <Volcano/Game/World.hpp>
#include <Volcano/Rpc/Common.hpp>

VOLCANO_RPC_BEGIN

class Stub {
public:
	Stub(void);
	virtual ~Stub(void);

private:
	Game::World m_world;
};

VOLCANO_RPC_END

#endif // VOLCANO_RPC_STUB_HPP
