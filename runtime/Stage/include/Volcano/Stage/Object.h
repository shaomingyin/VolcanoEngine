//
//
#ifndef VOLCANO_STAGE_OBJECT_H
#define VOLCANO_STAGE_OBJECT_H

#include <Volcano/STAGE/Common.h>

VOLCANO_STAGE_BEGIN

class Object {
public:
	Object() = default;
	virtual ~Object() = default;

public:
	sigslot::signal_st<const std::string&> setName;
	sigslot::signal_st<bool> setEnabled;
	sigslot::signal_st<bool> setVisible;
};

VOLCANO_STAGE_END

#endif // VOLCANO_STAGE_OBJECT_H
