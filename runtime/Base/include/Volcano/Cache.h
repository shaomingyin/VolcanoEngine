//
//
#ifndef VOLCANO_CACHE_H
#define VOLCANO_CACHE_H

#include <map>
#include <deque>
#include <optional>
#include <unordered_map>

#include <Volcano/Common.h>

VOLCANO_BEGIN

namespace CachePolicy {
	template <typename Key>
	class FIFO {
	public:
		void insert(Key key) {
			queue_.emplace_back(std::move(key));
		}

		void remove(Key key) {
			for (auto it = queue_.begin(); it != queue_.end(); ++it) {
				if (it == key) {
					queue_.erase(it);
					return;
				}
			}
		}

		std::optional<Key> removeLatestUnused() {
			std::optional<Key> key;
			if (!queue_.empty()) {
				key.emplace(std::move(queue_.front()));
				queue_.pop_front();
			}
			return key;
		}

	private:
		std::deque<Key> queue_;
	};

	template <typename Key, typename Clock>
	class LRU {
	public:
		void insert(Key key) {
			storage_.emplace(std::make_pair(Clock::now(), std::move(key)));
		}

		void remove(Key key) {
			for (auto it = storage_.begin(); it != storage_.end(); ++it) {
				if (it->second == key) {
					storage_.erase(it);
					return;
				}
			}
		}

		std::optional<Key> removeLatestUnused() {
			std::optional<Key> key;
			if (!storage_.empty()) {
				auto it = storage_.begin();
				key.emplace(std::move(it->second));
				storage_.erase(it);
			}
			return key;
		}

	private:
		std::map<typename Clock::time_point, Key> storage_;
	};

	template <typename Key, typename Clock>
	class LFU {
	public:
		void insert(Key key) {
			storage_.emplace({ Clock::now(), std::move(key) });
		}

		void remove(Key key) {
			for (auto it = storage_.begin(); it != storage_.end(); ++it) {
				if (it->second == key) {
					storage_.erase(it);
					return;
				}
			}
		}

		std::optional<Key> removeLatestUnused() {
			std::optional<Key> key;
			if (!storage_.empty()) {
				auto it = storage_.begin();
				key.emplace(std::move(it->second));
				storage_.erase(it);
			}
			return key;
		}

	private:
		std::map<typename Clock::time_point, Key> storage_;
	};
}

template <typename Key, typename Value, typename Policy = CachePolicy::LRU<Key, Clock>>
class Cache {
public:
	using KeyType = Key;
	using ValueType = Value;
	using PolicyType = Policy;

public:
	Cache(size_t max_slots = 100)
		: max_slots_(max_slots) {
	}

	virtual ~Cache() = default;

public:
	size_t maxSlots() const {
		return max_slots_;
	}

	void setMaxSlots(size_t n) {
		max_slots_ = n;
		tryToShrink();
	}

	size_t size() const {
		return slots_.size();
	}

	std::optional<Value> get(Key key) {
		auto it = slots_.find(key);
		if (it != slots_.end()) {
			return it->second;
		}
		return std::nullopt;
	}

	void insert(Key key, Value value) {
		slots_.emplace(key, std::move(value));
		policy_.insert(std::move(key));
		tryToShrink();
	}

	void remove(Key key) {
		auto it = slots_.find(key);
		if (it != slots_.end()) {
			slots_.erase(it);
			policy_.remove(std::move(key));
		}
	}

protected:
	void tryToShrink() {
		while (slots_.size() > max_slots_) {
			auto key = policy_.removeLatestUnused();
			if (key.has_value()) {
				auto it = slots_.find(key.value());
				if (it != slots_.end()) {
					slots_.erase(it);
				}
			}
		}
	}

private:
	//using Slots = std::unordered_map<Key, Value>;
	using Slots = std::map<Key, Value>;

private:
	size_t max_slots_;
	Slots slots_;
	Policy policy_;
};

VOLCANO_END

#endif // VOLCANO_CACHE_H
