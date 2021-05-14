//
//
#ifndef VOLCANO_RESOURCEMANAGER_HPP
#define VOLCANO_RESOURCEMANAGER_HPP

#include <memory>
#include <string_view>

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

using Resource = std::shared_ptr<IO>;

namespace ResourceManager {
	Resource get(std::string_view path);
	bool registerPackage(std::string_view nativePath, std::string_view path);
	bool unregisterPackage(std::string_view nativePath, std::string_view path);
	void gc(size_t cacheSize = 1024 * 1024 * 1024);
}

VOLCANO_END

#endif // VOLCANO_RESOURCEMANAGER_HPP
