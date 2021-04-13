//
//
#include <Volcano/Framework/Channel.hpp>

VOLCANO_FRAMEWORK_BEGIN

constexpr int CHANNEL_QUEUE_SIZE_SHIFT = 6;
constexpr int CHANNEL_QUEUE_SIZE = 1 << CHANNEL_QUEUE_SIZE_SHIFT;
constexpr int CHANNEL_QUEUE_MASK = CHANNEL_QUEUE_SIZE - 1;

Channel::Channel(void):
	m_queue(CHANNEL_QUEUE_SIZE),
	m_writePos(0),
	m_readPos(0)
{
}

Channel::~Channel(void)
{
}

bool Channel::write(std::function<bool (flexbuffers::Builder &)> writter)
{
	if ((m_writePos - m_readPos) < CHANNEL_QUEUE_SIZE) {
		if (writter(m_queue[(m_writePos)&CHANNEL_QUEUE_MASK])) {
			m_writePos += 1;
			return true;
		}
	}

	return false;
}

flexbuffers::Reference Channel::read(void)
{
	if (m_readPos < m_writePos)
		return flexbuffers::Reference(flexbuffers::GetRoot(m_queue[m_readPos++ & CHANNEL_QUEUE_MASK].GetBuffer()));

	return flexbuffers::Reference();
}

VOLCANO_FRAMEWORK_END
