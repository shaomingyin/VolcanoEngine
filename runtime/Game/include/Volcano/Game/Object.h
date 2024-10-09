//
//
#ifndef VOLCANO_GAME_OBJECT_H
#define VOLCANO_GAME_OBJECT_H

#include <string>

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class Object {
public:
	Object();
	virtual ~Object();

public:
	const std::string& objectName() const {
		return object_name_;
	}

	void setObjectName(const std::string& v) {
		object_name_ = v;
	}

private:
	std::string object_name_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_H
