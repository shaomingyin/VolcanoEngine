//
//
#include <algorithm>

#include <Volcano/System/Sound.hpp>

VOLCANO_SYSTEM_BEGIN

Sound::Sound(void):
	m_volumn(1.0f)
{
}

Sound::~Sound(void)
{
}

float Sound::soundVolumn(void) const
{
	return m_volumn;
}

void Sound::setSoundVolumn(float v)
{
	m_volumn = std::clamp(v, 0.0f, 1.0f);
}

VOLCANO_SYSTEM_END
