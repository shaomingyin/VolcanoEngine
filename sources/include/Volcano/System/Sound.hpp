//
//
#ifndef VOLCANO_SYSTEM_SOUND_HPP
#define VOLCANO_SYSTEM_SOUND_HPP

#include <Volcano/VM/Sound.hpp>
#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class Sound : public VM::Sound {
public:
	Sound(void);
	~Sound(void) override;

public:
	float soundVolumn(void) const override;
	void setSoundVolumn(float v) override;

private:
	float m_volumn;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SOUND_HPP
