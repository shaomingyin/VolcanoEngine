//
//
#include <memory>

#include <Volcano/Buffer.hpp>
#include <Volcano/ZipFileSystem.hpp>

VOLCANO_BEGIN

ZipFileSystem::ZipFileSystem(void)
{
	mz_zip_zero_struct(&m_zipArchive);
}

ZipFileSystem::~ZipFileSystem(void)
{
	mz_zip_end(&m_zipArchive);
}

bool ZipFileSystem::isWritable(void)
{
	return false;
}

bool ZipFileSystem::init(std::string_view nativePath)
{
	return mz_zip_reader_init_file(&m_zipArchive, nativePath.data(), 0);
}

FileSystem::Type ZipFileSystem::type(std::string_view path)
{
	if (!isPath(path))
		return Type::Invalid;

	mz_zip_archive_file_stat st;
	if (!stat(path.data(), st))
		return Type::Invalid;

	if (st.m_is_directory)
		return Type::Dir;

	return Type::File;
}

bool ZipFileSystem::remove(std::string_view path)
{
	return false;
}

bool ZipFileSystem::makeDir(std::string_view dirName)
{
	return false;
}

void ZipFileSystem::enumDir(std::string_view dirName, StringList &out, bool isRecursive)
{
	// TODO
}

IO *ZipFileSystem::openFile(std::string_view fileName, int modes)
{
	mz_zip_archive_file_stat st;
	if (!stat(fileName.data(), st))
		return nullptr;

	if (st.m_is_directory)
		return nullptr;

	auto buf = std::make_unique<Buffer>(st.m_uncomp_size);
	if (!buf)
		return nullptr;

	if (st.m_uncomp_size > 0) {
		if (!mz_zip_reader_extract_to_mem(&m_zipArchive,
			st.m_file_index, buf->byteArray().data(), st.m_uncomp_size, 0))
		return false;
	}

	return buf.release();
}

bool ZipFileSystem::stat(std::string_view path, mz_zip_archive_file_stat &out)
{
	auto index = mz_zip_reader_locate_file(&m_zipArchive, path.data(), nullptr, 0);
	if (index < 0)
		return false;

	return mz_zip_reader_file_stat(&m_zipArchive, index, &out);
}

VOLCANO_END
