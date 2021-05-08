//
//
#include <filesystem>

#include <Volcano/ZipFileSystem.hpp>
#include <Volcano/RootFileSystem.hpp>

VOLCANO_BEGIN

RootFileSystem::RootFileSystem(void)
{
}

RootFileSystem::~RootFileSystem(void)
{
}

bool RootFileSystem::init(std::string_view nativePath)
{
	if (!FileSystem::init(nativePath))
		return false;

	return true;
}

FileSystem::Type RootFileSystem::type(std::string_view path)
{
	Type ret = FileSystem::type(path);
	if (ret != Type::Invalid)
		return ret;

	// TODO

	return ret;
}

void RootFileSystem::enumDir(std::string_view dirName, StringList &out, bool isRecursive)
{
	FileSystem::enumDir(dirName, out, isRecursive);

	// TODO
}

IO *RootFileSystem::openFile(std::string_view fileName, int modes)
{
	IO *file = FileSystem::openFile(fileName, modes);
	if (file != nullptr)
		return file;

	// TODO

	return nullptr;
}

bool RootFileSystem::mount(std::string_view nativePath, std::string_view path)
{
	if (!isPath(path))
		return false;

	for (auto &info: m_mountInfos) {
		if (info.fs->nativePath() == nativePath && info.path == path)
			return false;
	}

	FileSystemPtr fs;

	if (std::filesystem::is_directory(nativePath))
		fs = std::make_unique<FileSystem>();
	else if (std::filesystem::is_regular_file(nativePath))
		fs = std::make_unique<ZipFileSystem>();
	else
		return false;

	if (!fs || !fs->init(nativePath))
		return false;

	auto &info = m_mountInfos.emplace_front();
	info.fs = std::move(fs);
	info.path = path;

	return false;
}

bool RootFileSystem::umount(std::string_view nativePath, std::string_view path)
{
	if (!isPath(path))
		return false;

	for (auto it = m_mountInfos.begin(); it != m_mountInfos.end(); ++it) {
		if (it->fs->nativePath() == nativePath && it->path == path) {
			m_mountInfos.erase(it);
			return true;
		}
	}

	return false;
}

VOLCANO_END
