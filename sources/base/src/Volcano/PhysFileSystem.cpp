//
//
#include <memory>

#include <Volcano/PhysFile.hpp>
#include <Volcano/PhysFileSystem.hpp>

VOLCANO_BEGIN

static PHYSFS_EnumerateCallbackResult enumDirCallback(void *data, const char *origdir, const char *fname)
{
	auto pout = reinterpret_cast<StringList *>(data);
	pout->push_back(fname);

	return PHYSFS_ENUM_OK;
}

static PHYSFS_EnumerateCallbackResult enumDirCallbackRecursived(void *data, const char *origdir, const char *fname)
{
	auto pout = reinterpret_cast<StringList *>(data);
	pout->push_back(fname);

	std::string fullPath = origdir;
	fullPath += "/";
	fullPath += fname;

	if (!PHYSFS_isDirectory(fullPath.c_str()))
		return PHYSFS_ENUM_OK;

	if (!PHYSFS_enumerate(fullPath.c_str(), enumDirCallbackRecursived, data))
		return PHYSFS_ENUM_ERROR;

	return PHYSFS_ENUM_OK;
}

PhysFileSystem::PhysFileSystem(void)
{
}

PhysFileSystem::~PhysFileSystem(void)
{
}

FileSystem::Type PhysFileSystem::type(std::string_view path)
{
	if (!checkPath(path))
		return Type::Unknown;

	PHYSFS_Stat st;

	if (!PHYSFS_stat(path.data(), &st))
		return Type::Unknown;

	if (st.filetype == PHYSFS_FILETYPE_DIRECTORY)
		return Type::Dir;

	if (st.filetype == PHYSFS_FILETYPE_REGULAR)
		return Type::File;
	
	return Type::Unknown;
}

bool PhysFileSystem::remove(std::string_view path)
{
	if (!checkPath(path))
		return false;

	return PHYSFS_delete(path.data());
}

bool PhysFileSystem::makeDir(std::string_view dirName)
{
	if (!checkPath(dirName))
		return false;

	return PHYSFS_mkdir(dirName.data());
}

void PhysFileSystem::enumDir(std::string_view dirName, StringList &out, bool isRecursive)
{
	if (!checkPath(dirName))
		return;

	if (isRecursive)
		PHYSFS_enumerate(dirName.data(), enumDirCallbackRecursived, &out);
	else
		PHYSFS_enumerate(dirName.data(), enumDirCallback, &out);
}

IO *PhysFileSystem::openFile(std::string_view fileName, int modes)
{
	if (!checkPath(fileName))
		return nullptr;

	if (type(fileName) != Type::File)
		return nullptr;

	auto file = std::make_unique<PhysFile>(fileName);
	if (!file || !file->open(modes))
		return nullptr;

	return file.release();
}

VOLCANO_END
