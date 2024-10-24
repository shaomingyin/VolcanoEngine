//
//
#ifndef VOLCANO_SYSTEM_BASE_H
#define VOLCANO_SYSTEM_BASE_H

#include <Volcano/ResourceManager.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Base {
public:
	Base() = default;
	virtual ~Base() = default;

public:
	ResourceManager& resourceManager() {
		return resource_manager_;
	}

	const ResourceManager& resourceManager() const {
		return resource_manager_;
	}

private:
	ResourceManager resource_manager_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_BASE_H
