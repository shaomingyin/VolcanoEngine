//
//
#include <filesystem>

#include <Volcano/File.hpp>

VOLCANO_BEGIN

File::File(std::string_view fileName):
	m_fileName(fileName),
	m_fp(nullptr)
{
}

File::~File(void)
{
	if (m_fp != nullptr)
		close();
}

int File::flags(void)
{
	return FlagReadable | FlagWritable | FlagSeekable /* | FlagMappable */;
}

int64_t File::size(void)
{
	return std::filesystem::file_size(m_fileName);
}

bool File::open(int modes)
{
	VOLCANO_ASSERT(m_fp == nullptr);

	if (!IO::open(modes))
		return false;

	std::string cModes("b");

	if (modes & ModeRead)
		cModes += 'r';

	if (modes & ModeWrite)
		cModes += 'w';

	if (modes & ModeAppend)
		cModes += 'a';

	if (modes & ModeNewOnly)
		cModes += '+';

	m_fp = fopen(m_fileName.c_str(), cModes.c_str());
	if (m_fp == nullptr) {
		IO::close();
		return false;
	}

	return true;
}

void File::close(void)
{
	VOLCANO_ASSERT(m_fp != nullptr);

	fclose(m_fp);
	m_fp = nullptr;
}

const std::string &File::fileName(void) const
{
	return m_fileName;
}

int64_t File::pos(void)
{
	VOLCANO_ASSERT(m_fp != nullptr);

	return ftell(m_fp);
}

bool File::setPos(int64_t pos)
{
	VOLCANO_ASSERT(m_fp != nullptr);

	return false; // todo fseeko64(m_fp, pos, SEEK_SET) == pos;
}

int64_t File::readData(void *buf, int64_t len)
{
	VOLCANO_ASSERT(m_fp != nullptr);

	return fread(buf, 1, len, m_fp);
}

int64_t File::writeData(const void *buf, int64_t len)
{
	VOLCANO_ASSERT(m_fp != nullptr);

	return fwrite(buf, 1, len, m_fp);
}

void *File::createMap(int64_t offset, int64_t len)
{
	return nullptr;
}

void File::releaseMap(void *p, int64_t len)
{
}

VOLCANO_END
