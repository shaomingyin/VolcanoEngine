//
//
#include <Volcano/Dir.hpp>

VOLCANO_BEGIN

Dir::Dir(void)
{
}

Dir::Dir(std::string_view path):
	m_path(path)
{
}

Dir::~Dir(void)
{
}

const std::string &Dir::path(void) const
{
	return m_path;
}

bool Dir::mkdir(std::string_view path)
{
	return PHYSFS_mkdir(path.data());
}

bool Dir::isDir(std::string_view path)
{
	PHYSFS_Stat out;

	if (!PHYSFS_stat(path.data(), &out))
		return false;

	return (out.filetype == PHYSFS_FILETYPE_DIRECTORY);
}

VOLCANO_END
