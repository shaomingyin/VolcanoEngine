//
//
#ifndef VOLCANO_LAUNCHER_MULTITHREADEDSINGLEPLAYER_HPP
#define VOLCANO_LAUNCHER_MULTITHREADEDSINGLEPLAYER_HPP

#include <future>
#include <thread>

#include <Volcano/Launcher/Player.hpp>

VOLCANO_LAUNCHER_BEGIN

class MultiThreadedSinglePlayer : public Player {
public:
	MultiThreadedSinglePlayer(FileSystem *fs);
	virtual ~MultiThreadedSinglePlayer(void);

public:
	bool init(void) override;

protected:
	void frame(Duration elapsed) override;

private:
	void threadMain(std::promise<bool> *initPromise);

private:
	std::thread m_thread;
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_MULTITHREADEDSINGLEPLAYER_HPP
