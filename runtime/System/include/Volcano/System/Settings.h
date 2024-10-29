//
//
#ifndef VOLCANO_SYSTEM_SETTINGS_H
#define VOLCANO_SYSTEM_SETTINGS_H

#include <filesystem>

#include <nlohmann/json.hpp>

#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Settings {
public:
	Settings() = default;
	virtual ~Settings() = default;

public:
	template <typename T>
	auto get(const std::filesystem::path& path) const {
		return find(path).get<T>();
	}

	template <typename T>
	auto get(const std::filesystem::path& path, T&& def) const {
		auto& v = find(path);
		if (v.is_null()) {
			return def;
		}
		return v.get<T>();
	}

	void update();

private:
	const nlohmann::json& find(const std::filesystem::path& path) const;

private:
	nlohmann::json json_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SETTINGS_H
