//
//
#include <Volcano/System/Settings.h>

VOLCANO_SYSTEM_BEGIN

void Settings::update() {
}

const nlohmann::json& Settings::find(const std::filesystem::path& path) const {
	auto gpath = normalizedPath(path);
	static nlohmann::json null_json;
	if (gpath.empty()) {
		return null_json;
	}
	std::filesystem::path::iterator path_it = gpath.begin();
	if (gpath.is_absolute()) {
		++path_it;
	}
	if (*path_it == "/") {
		++path_it;
	}
	const nlohmann::json* p = &json_;
	while (path_it != gpath.end()) {
		auto it = p->find(path_it->string());
		if (it == p->end()) {
			return null_json;
		}
		p = &it.value();
		++path_it;
	}
	return *p;
}

VOLCANO_SYSTEM_END
