//
//
#ifndef VOLCANO_WORLD_COMPONENT_H
#define VOLCANO_WORLD_COMPONENT_H

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Component {
public:
	Component();
	virtual ~Component();

public:
	virtual void loadFromJson(const nlohmann::json& j);
	virtual void saveToJson(nlohmann::json& j);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COMPONENT_H
