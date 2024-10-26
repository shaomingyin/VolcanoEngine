//
//
#include <Volcano/System/JsonUtils.h>

VOLCANO_SYSTEM_BEGIN

namespace JsonUtils {
	const nlohmann::json& find(const nlohmann::json& json, const std::filesystem::path& path) {
		static nlohmann::json null_json;
		if (path.empty()) {
			return null_json;
		}
		std::filesystem::path::iterator path_it = path.begin();
		if (path.is_absolute()) {
			++path_it;
		}
		if (*path_it == "/") {
			++path_it;
		}
		const nlohmann::json* p = &json;
		while (path_it != path.end()) {
			auto it = p->find(path_it->string());
			if (it == p->end()) {
				return null_json;
			}
			p = &it.value();
			++path_it;
		}
		return *p;
	}
}
VOLCANO_SYSTEM_END
