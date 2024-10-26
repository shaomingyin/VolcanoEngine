//
//
#ifndef VOLCANO_SYSTEM_JSONUTILS_H
#define VOLCANO_SYSTEM_JSONUTILS_H

#include <filesystem>

#include <nlohmann/json.hpp>

#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

namespace JsonUtils {
	const nlohmann::json& find(const nlohmann::json& json, const std::filesystem::path& path);

	template <typename T>
	auto read(const nlohmann::json& json, const std::filesystem::path& path) {
		return find(json, path).get<T>();
	}

	template <typename T>
	auto read(const nlohmann::json& json, const std::filesystem::path& path, T&& def) {
		auto& v = find(json, path);
		if (v.is_null()) {
			return def;
		}
		return v.get<T>();
	}
}

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_JSONUTILS_H
