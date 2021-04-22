//
//
#ifndef VOLCANO_VM_SOUND_HPP
#define VOLCANO_VM_SOUND_HPP

#include <Volcano/VM/Common.hpp>

VOLCANO_VM_BEGIN

class Sound {
public:
	Sound(void) = default;
	virtual ~Sound(void) = default;

public:
	virtual float soundVolumn(void) const = 0;
	virtual void setSoundVolumn(float v) = 0;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_SOUND_HPP
