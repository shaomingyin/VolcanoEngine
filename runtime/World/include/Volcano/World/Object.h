//
//
#ifndef VOLCANO_WORLD_OBJECT_H
#define VOLCANO_WORLD_OBJECT_H

#include <Volcano/WORLD/Common.h>

VOLCANO_WORLD_BEGIN

class Object {
public:
	Object() = default;
	virtual ~Object() = default;

public:
	sigslot::signal_st<const std::string&> setName;
	sigslot::signal_st<bool> setEnabled;
	sigslot::signal_st<bool> setVisible;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_OBJECT_H
