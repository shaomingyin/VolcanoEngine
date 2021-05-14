//
//
#include <Volcano/ResourceManager.hpp>

VOLCANO_BEGIN

namespace ResourceManager {
	Resource get(std::string_view path)
	{
		return nullptr;
	}

	bool registerPackage(std::string_view nativePath, std::string_view path)
	{
		return false;
	}

	bool unregisterPackage(std::string_view nativePath, std::string_view path)
	{
		return false;
	}

	void gc(size_t cacheSize)
	{

	}
}

VOLCANO_END
