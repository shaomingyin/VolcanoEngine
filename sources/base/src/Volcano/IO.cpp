//
//
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

// IO::Mapper

IO::Mapper::Mapper(Mapper &&that) :
	m_io(that.m_io),
	m_data(that.m_data),
	m_size(that.m_size)
{
	that.m_data = nullptr;
	that.m_size = 0;
}

IO::Mapper::Mapper(IO &io, void *data, int64_t size):
	m_io(io),
	m_data(data),
	m_size(size)
{
}

IO::Mapper::~Mapper(void)
{
	if (isValid())
		m_io.releaseMap(m_data, m_size);
}

int64_t IO::Mapper::size(void) const
{
	return m_size;
}

void *IO::Mapper::data(void)
{
	return m_data;
}

bool IO::Mapper::isValid(void) const
{
	return (m_data != nullptr && m_size > 0);
}

IO &IO::Mapper::io(void)
{
	return m_io;
}

// IO

IO::IO(void):
	m_modes(0)
{
}

IO::~IO(void)
{
	if (isOpen())
		close();
}

bool IO::open(int modes)
{
	VOLCANO_ASSERT(!isOpen());
	VOLCANO_ASSERT(modes != 0);

	int localFlags = flags();

	if (modes & ModeRead) {
		if (!(localFlags & FlagReadable))
			return false;
	}

	if (modes & (ModeWrite | ModeAppend | ModeTruncate | ModeNewOnly)) {
		if (!(localFlags & FlagWritable))
			return false;
		if ((modes & ModeWrite) && (modes & ModeAppend))
			return false;
		if ((modes & ModeTruncate) && (modes & (ModeAppend & ModeNewOnly)))
			return false;
	}

	m_modes = modes;

	return true;
}

void IO::close(void)
{
	VOLCANO_ASSERT(isOpen());

	m_modes = 0;
}

bool IO::isOpen(void) const
{
	return !!m_modes;
}

int IO::modes(void) const
{
	VOLCANO_ASSERT(isOpen());

	return m_modes;
}

int64_t IO::tell(void)
{
	VOLCANO_ASSERT(isOpen());

	return pos();
}

int64_t IO::seek(int64_t offset, SeekMode seekMode)
{
	VOLCANO_ASSERT(flags() & FlagSeekable);
	VOLCANO_ASSERT(isOpen());

	int64_t newPos;

	switch (seekMode) {
	case SeekMode::Set:
		newPos = offset;
		break;
	case SeekMode::Cur:
		newPos = pos() + offset;
		break;
	case SeekMode::End:
		newPos = size() + offset;
		break;
	default:
		newPos = -1;
		break;
	}

	if (newPos < 0 || !setPos(newPos))
		return -1;

	return true;
}

int64_t IO::read(void *buf, int64_t len)
{
	VOLCANO_ASSERT(flags() & FlagReadable);
	VOLCANO_ASSERT(isOpen());
	VOLCANO_ASSERT(buf != nullptr);
	VOLCANO_ASSERT(len > 0);

	return readData(buf, len);
}

int64_t IO::write(const void *buf, int64_t len)
{
	VOLCANO_ASSERT(flags() & FlagWritable);
	VOLCANO_ASSERT(isOpen());
	VOLCANO_ASSERT(buf != nullptr);
	VOLCANO_ASSERT(len > 0);

	return writeData(buf, len);
}

IO::Mapper IO::map(int64_t offset, int64_t len)
{
	VOLCANO_ASSERT(flags() & FlagMappable);
	VOLCANO_ASSERT(isOpen());

	return Mapper(*this, createMap(offset, len), len);
}

VOLCANO_END
