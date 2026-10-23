//
//
#ifndef VOLCANO_CACHE_H
#define VOLCANO_CACHE_H

#include <algorithm>
#include <cstddef>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <utility>

#include <Volcano/Common.h>

VOLCANO_BEGIN

// Access to a shared cache must be synchronized by its caller.
template <typename T>
class Cache {
public:
    using Type = T;
    using Pointer = std::shared_ptr<T>;
    using ToKey = std::function<std::string (const T&)>;

    Cache(std::size_t size = 8) : size_(size) {
    }

    Cache(const Cache&) = delete;

    Cache(Cache&&) = default;

    virtual ~Cache() = default;

    Cache& operator=(const Cache&) = delete;

    Cache& operator=(Cache&&) = default;

    void clear() noexcept {
        storage_.clear();
    }

    Pointer get(const std::string& key) {
        // shortcut: linear lookup suits small caches; add a key index if large caches need it.
        const auto found = std::find_if(storage_.begin(), storage_.end(), [&](const auto& entry) {
            return entry.first == key;
        });
        if (found != storage_.end()) {
            storage_.splice(storage_.begin(), storage_, found);
            return found->second;
        }
        auto value = load(key);
        if (value && size_ != 0) {
            storage_.emplace_front(key, value);
            if (storage_.size() > size_) {
                storage_.pop_back();
            }
        }
        return value;
    }

protected:
    virtual Pointer load(const std::string& key) = 0;

private:
    std::size_t size_;
    std::list<std::pair<std::string, Pointer>> storage_;
};

VOLCANO_END

#endif // VOLCANO_CACHE_H
