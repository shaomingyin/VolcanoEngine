//
//
#ifndef VOLCANO_GUI_CACHE_H
#define VOLCANO_GUI_CACHE_H

#include <map>
#include <memory>
#include <string>
#include <chrono>
#include <functional>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Font.hpp>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

template <typename T>
class Cache: public sf::NonCopyable {
public:
	using Type = T;
	using Pointer = std::shared_ptr<T>;
	using ToKey = std::function<std::string (const T&)>;

public:
	Cache(size_t size = 8)
		: size_(size) {
	}

	virtual ~Cache() = default;

public:
	void clear() {
		storage_.clear();
	}

	Pointer get(const std::string& key) {
		auto font = find(key);
		if (!font) {
			font = load(key);
			storage_.emplace(Clock::now(), font);
			gc();
		}
		return font;
	}

protected:
	using Clock = std::chrono::steady_clock;
	using Storage = std::map<Clock::time_point, Pointer>;

	Pointer find(const std::string& family) const {
		for (const auto it: storage_) {
			if (it.second->getInfo().family == family) {
				return it.second;
			}
		}
		return nullptr;
	}

	void gc() noexcept {
		if (storage_.size() > size_) {
			storage_.erase(storage_.begin());
		}
		// TODO
	}

	virtual Pointer load(const std::string& family) = 0;

private:
	const size_t size_;
	Storage storage_;
};

using FontCache = Cache<sf::Font>;

VOLCANO_GUI_END

#endif // VOLCANO_GUI_CACHE_H
