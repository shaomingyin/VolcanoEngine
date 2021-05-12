//
//
#ifndef VOLCANO_LAUNCHER_MULTIPLAYER_HPP
#define VOLCANO_LAUNCHER_MULTIPLAYER_HPP

#include <Volcano/Rpc/Proxy.hpp>
#include <Volcano/Launcher/Common.hpp>
#include <Volcano/Launcher/Player.hpp>

VOLCANO_LAUNCHER_BEGIN

class MultiPlayer: public Player {
public:
	MultiPlayer(FileSystem *fs);
	virtual ~MultiPlayer(void);

public:
	bool init(void) override;
	void handleEvent(const SDL_Event &evt) override;

protected:
	void frame(Duration elapsed) override;

private:
	Rpc::Proxy m_proxy;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_MULTIPLAYER_HPP
