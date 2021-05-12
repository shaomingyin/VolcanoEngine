//
//
#include <cctype>
#include <memory>
#include <filesystem>

#include <Volcano/File.hpp>
#include <Volcano/NativeFileSystem.hpp>

VOLCANO_BEGIN

NativeFileSystem::NativeFileSystem(void)
{
}

NativeFileSystem::~NativeFileSystem(void)
{
}

bool NativeFileSystem::init(std::string_view nativePath)
{
	if (!std::filesystem::is_directory(nativePath))
		return false;

	m_nativePath = nativePath;

	return true;
}

const std::string &NativeFileSystem::nativePath(void) const
{
	return m_nativePath;
}

FileSystem::Type NativeFileSystem::type(std::string_view path)
{
	if (!checkPath(path))
		return Type::Unknown;

	std::string fullPath = m_nativePath + path.data();

	if (std::filesystem::is_directory(fullPath))
		return Type::Dir;

	if (std::filesystem::is_regular_file(fullPath))
		return Type::File;

	return Type::Unknown;
}

bool NativeFileSystem::remove(std::string_view path)
{
	if (!checkPath(path))
		return false;

	return std::filesystem::remove(m_nativePath + path.data());
}

bool NativeFileSystem::makeDir(std::string_view dirName)
{
	if (type(dirName) != Type::Dir)
		return false;

	return std::filesystem::create_directory(m_nativePath + dirName.data());
}

void NativeFileSystem::enumDir(std::string_view dirName, StringList &out, bool isRecursive)
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

IO *NativeFileSystem::openFile(std::string_view fileName, int modes)
{
	if (!checkPath(fileName))
		return nullptr;

	auto file = std::make_unique<File>(m_nativePath + fileName.data());
	if (!file || !file->open(modes))
		return nullptr;

	return file.release();
}

VOLCANO_END
