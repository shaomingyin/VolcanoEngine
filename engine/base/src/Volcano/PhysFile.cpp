//
//
#include <Volcano/PhysFile.hpp>

VOLCANO_BEGIN

PhysFile::PhysFile(std::string_view fileName):
	m_fileName(fileName),
	m_fp(nullptr)
{
}

PhysFile::~PhysFile(void)
{
	if (m_fp != nullptr)
		PHYSFS_close(m_fp);
}

int PhysFile::flags(void)
{
	int ret = FlagReadable | FlagSeekable;

	if (PHYSFS_getWriteDir() != nullptr)
		ret |= FlagWritable;

	return ret;
}

int64_t PhysFile::size(void)
{
	if (m_fp != nullptr)
		return PHYSFS_fileLength(m_fp);

	PHYSFS_Stat st;

	if (!PHYSFS_stat(m_fileName.c_str(), &st))
		return -1;

	if (st.filetype != PHYSFS_FILETYPE_REGULAR)
		return -1;

	return st.filesize;
}

bool PhysFile::open(int modes)
{
	VOLCANO_ASSERT(m_fp == nullptr);

	if (!IO::open(modes))
		return false;

	if (modes & ModeRead) {
		if (modes & (ModeWrite | ModeAppend | ModeTruncate | ModeNewOnly)) {
			IO::close();
			return false;
		}
	}

	if ((modes & ModeWrite) && (modes & ModeAppend)) {
		IO::close();
		return false;
	}

	if (modes & ModeNewOnly) {
		if (PHYSFS_exists(m_fileName.c_str()))
			return false;
	}

	if (modes & ModeRead)
		m_fp = PHYSFS_openRead(m_fileName.c_str());
	else if (modes & ModeWrite)
		m_fp = PHYSFS_openWrite(m_fileName.c_str());
	else if (modes & ModeAppend)
		m_fp = PHYSFS_openAppend(m_fileName.c_str());
	else
		return false;

	if (m_fp == nullptr)
		return false;

	return true;
}

void PhysFile::close(void)
{
	VOLCANO_ASSERT(m_fp != nullptr);

	PHYSFS_close(m_fp);
}

const std::string &PhysFile::fileName(void) const
{
	return m_fileName;
}

int64_t PhysFile::pos(void)
{
	VOLCANO_ASSERT(m_fp != nullptr);

	return PHYSFS_tell(m_fp);
}

bool PhysFile::setPos(int64_t pos)
{
	VOLCANO_ASSERT(m_fp != nullptr);

	return PHYSFS_seek(m_fp, pos);
}

int64_t PhysFile::readData(void *buf, int64_t len)
{
	VOLCANO_ASSERT(m_fp != nullptr);
	VOLCANO_ASSERT(buf != nullptr);
	VOLCANO_ASSERT(len > 0);

	return PHYSFS_readBytes(m_fp, buf, len);
}

int64_t PhysFile::writeData(const void *buf, int64_t len)
{
	VOLCANO_ASSERT(m_fp != nullptr);
	VOLCANO_ASSERT(buf != nullptr);
	VOLCANO_ASSERT(len > 0);

	return PHYSFS_writeBytes(m_fp, buf, len);
}

void *PhysFile::createMap(int64_t offset, int64_t len)
{
	return nullptr;
}

void PhysFile::releaseMap(void *p, int64_t len)
{
	VOLCANO_ASSERT(m_fp != nullptr);
}

VOLCANO_END
