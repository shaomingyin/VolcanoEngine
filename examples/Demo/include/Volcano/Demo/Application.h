//
//
#ifndef VOLCANO_DEMO_APPLICATION_H
#define VOLCANO_DEMO_APPLICATION_H

#include <string>

#include <Volcano/Player/Local.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Demo/Common.h>

VOLCANO_DEMO_BEGIN

class Application {
public:
	Application(std::string manifest_path);

public:
	void run();

private:
	std::string current_scene_path_;
	World::Scene scene_;
	Player::Local player_;
};

VOLCANO_DEMO_END

#endif // VOLCANO_DEMO_APPLICATION_H
