//
//
#include <algorithm>

#include <Volcano/System/Sound.hpp>

VOLCANO_SYSTEM_BEGIN

Sound::Sound(void):
	m_volume(1.0f)
{
}

Sound::~Sound(void)
{
}

float Sound::soundVolume(void) const
{
	return m_volume;
}

void Sound::setSoundVolume(float v)
{
	m_volume = std::clamp(v, 0.0f, 1.0f);
}

VOLCANO_SYSTEM_END
