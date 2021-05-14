//
//
#include <cctype>

#include <Volcano/FileSystem.hpp>

VOLCANO_BEGIN

bool FileSystem::exists(std::string_view path)
{
	if (!checkPath(path))
		return false;

	return (type(path) != Type::Unknown);
}

bool FileSystem::checkPath(std::string_view path)
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
		}
		else if (!isNameChar(path[i]))
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
