//
//
#include <cctype>
#include <memory>
#include <filesystem>

#include <Volcano/File.hpp>
#include <Volcano/FileSystem.hpp>

VOLCANO_BEGIN

FileSystem::FileSystem(void)
{
}

FileSystem::~FileSystem(void)
{
}

bool FileSystem::isWritable(void)
{
	return true;
}

bool FileSystem::init(std::string_view nativePath)
{
	if (!std::filesystem::is_directory(nativePath))
		return false;

	m_nativePath = nativePath;

	return true;
}

const std::string &FileSystem::nativePath(void) const
{
	return m_nativePath;
}

bool FileSystem::isExists(std::string_view path)
{
	if (!isPath(path))
		return false;

	return (type(path) != Type::Invalid);
}

FileSystem::Type FileSystem::type(std::string_view path)
{
	if (!isPath(path))
		return Type::Invalid;

	std::string fullPath = m_nativePath + path.data();

	if (std::filesystem::is_directory(fullPath))
		return Type::Dir;

	if (std::filesystem::is_regular_file(fullPath))
		return Type::File;

	return Type::Invalid;
}

bool FileSystem::remove(std::string_view path)
{
	if (!isPath(path))
		return false;

	return std::filesystem::remove(m_nativePath + path.data());
}

bool FileSystem::makeDir(std::string_view dirName)
{
	if (type(dirName) != Type::Dir)
		return false;

	return std::filesystem::create_directory(m_nativePath + dirName.data());
}

void FileSystem::enumDir(std::string_view dirName, StringList &out, bool isRecursive)
{
	out.clear();

	if (type(dirName) != Type::Dir)
		return;

	std::string fullPath = m_nativePath + dirName.data();

	if (isRecursive) {
		for (auto &p: std::filesystem::recursive_directory_iterator(fullPath))
			out.push_back(p.path().string());
	} else {
		for (auto &p : std::filesystem::directory_iterator(fullPath))
			out.push_back(p.path().string());
	}
}

IO *FileSystem::openFile(std::string_view fileName, int modes)
{
	if (!isPath(fileName))
		return nullptr;

	auto file = std::make_unique<File>(m_nativePath + fileName.data());
	if (!file || !file->open(modes))
		return nullptr;

	return file.release();
}

bool FileSystem::isPath(std::string_view path)
{
	size_t size = path.size();
	if (size < 1)
		return false;

	if (path[0] != '/')
		return false;

	bool slashed = true;

	for (size_t i = 1; i < size; ++i) {
		if (path[i] == '/') {
			if (slashed)
				return false;
			slashed = true;
		} else if (!isNameChar(path[i]))
			return false;
		else
			slashed = false;
	}

	return true;
}

bool FileSystem::isNameChar(char c)
{
	return std::isalnum(c) || c == '.' || c == '_';
}

VOLCANO_END
