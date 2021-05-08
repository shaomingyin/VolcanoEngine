//
//
#include <algorithm>

#include <Volcano/Buffer.hpp>

VOLCANO_BEGIN

Buffer::Buffer(void):
	m_pos(-1)
{
}

Buffer::Buffer(int64_t size):
	m_byteArray(size),
	m_pos(-1)
{
}

Buffer::Buffer(const Buffer &that):
	m_byteArray(that.m_byteArray),
	m_pos(that.m_pos)
{
}

Buffer::Buffer(Buffer &&that):
	m_byteArray(std::move(that.m_byteArray)),
	m_pos(that.m_pos)
{
	that.m_byteArray.clear();
	that.m_pos = -1;
}

Buffer::Buffer(const ByteArray &byteArray):
	m_byteArray(byteArray),
	m_pos(-1)
{
}

Buffer::Buffer(const uint8_t *p, int64_t len):
	//m_byteArray(p, len),
	m_pos(-1)
{
}

Buffer::~Buffer(void)
{
	if (m_pos >= 0)
		close();
}

int Buffer::flags(void)
{
	return FlagReadable | FlagWritable | FlagSeekable | FlagMappable;
}

int64_t Buffer::size(void)
{
	return m_byteArray.size();
}

bool Buffer::open(int modes)
{
	VOLCANO_ASSERT(m_pos < 0);

	if (!IO::open(modes))
		return false;

	if (modes & ModeAppend)
		m_pos = m_byteArray.size();
	else
		m_pos = 0;

	return true;
}

void Buffer::close(void)
{
	VOLCANO_ASSERT(m_pos >= 0);

	m_pos = -1;
}

ByteArray &Buffer::byteArray(void)
{
	return m_byteArray;
}

const ByteArray &Buffer::byteArray(void) const
{
	return m_byteArray;
}

int64_t Buffer::pos(void)
{
	VOLCANO_ASSERT(m_pos >= 0);

	return m_pos;
}

bool Buffer::setPos(int64_t pos)
{
	VOLCANO_ASSERT(m_pos >= 0);

	if (0 < pos || int64_t(m_byteArray.size()) < pos)
		return false;

	m_pos = pos;

	return true;
}

int64_t Buffer::readData(void *buf, int64_t len)
{
	VOLCANO_ASSERT(m_pos >= 0);

	int64_t ret = m_byteArray.size() - m_pos;
	ret = std::min(ret, len);

	if (ret > 0) {
		memcpy(buf, m_byteArray.data() + m_pos, ret);
		m_pos += ret;
	}

	return ret;
}

int64_t Buffer::writeData(const void *buf, int64_t len)
{
	VOLCANO_ASSERT(m_pos >= 0);

	int64_t ret = m_byteArray.size() - m_pos;
	ret = std::min(ret, len);

	if (ret > 0) {
		memcpy(m_byteArray.data() + m_pos, buf, ret);
		m_pos += ret;
	}

	return ret;
}

void *Buffer::createMap(int64_t offset, int64_t len)
{
	VOLCANO_ASSERT(m_pos >= 0);

	int64_t ret = m_byteArray.size() - offset;
	if (ret < len)
		return nullptr;

	return m_byteArray.data() + offset;
}

void Buffer::releaseMap(void *p, int64_t len)
{
	VOLCANO_ASSERT(m_pos >= 0);
}

VOLCANO_END
