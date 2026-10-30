#ifndef VOLCANO_CACHE_H
#define VOLCANO_CACHE_H

#include <cstddef>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <Volcano/Common.h>

VOLCANO_BEGIN

template <typename T, typename Identifier = std::string>
class Cache final {
public:
    using Pointer = std::shared_ptr<T>;
    using Entries = std::map<Identifier, const T*>;

    class Policy {
    public:
        virtual ~Policy() = default;
        virtual void access(const Identifier& identifier, const T& resource) = 0;
        virtual std::vector<Identifier> selectEvictions(const Entries& entries) = 0;
        virtual void remove(const Identifier& identifier) noexcept = 0;
        virtual void clear() noexcept = 0;
    };

    explicit Cache(std::unique_ptr<Policy> policy) : policy_(std::move(policy)) {
        if (!policy_) {
            throw std::invalid_argument("Cache requires a policy");
        }
    }

    Cache(const Cache&) = delete;
    Cache& operator=(const Cache&) = delete;
    Cache(Cache&&) = delete;
    Cache& operator=(Cache&&) = delete;

    template <typename Loader> Pointer load(const Identifier& identifier, Loader&& loader) {
        // Keep the result and retired resources alive until after the lock is released.
        Pointer resource;
        std::vector<Pointer> retired;
        {
            const std::lock_guard lock(mutex_);
            resource = find(identifier);
            if (resource) {
                policy_->access(identifier, *resource);
                entries_.at(identifier).retained = resource;
                collect(retired);
                return resource;
            }
        }

        resource = std::invoke(std::forward<Loader>(loader));
        if (!resource) {
            throw std::runtime_error("Cache loader returned null");
        }

        const std::lock_guard lock(mutex_);
        auto existing = find(identifier);
        if (existing) {
            policy_->access(identifier, *existing);
            entries_.at(identifier).retained = existing;
            collect(retired);
            return existing;
        }
        const auto inserted = entries_.emplace(identifier, Entry{resource, resource}).first;
        try {
            policy_->access(identifier, *resource);
        } catch (...) {
            entries_.erase(inserted);
            throw;
        }
        collect(retired);
        return resource;
    }

    void gc() {
        std::vector<Pointer> retired;
        const std::lock_guard lock(mutex_);
        collect(retired);
    }

    void clear() {
        std::map<Identifier, Entry> retired;
        const std::lock_guard lock(mutex_);
        policy_->clear();
        entries_.swap(retired);
    }

private:
    struct Entry {
        std::weak_ptr<T> resource;
        Pointer retained;
    };

    Pointer find(const Identifier& identifier) {
        const auto found = entries_.find(identifier);
        if (found == entries_.end()) {
            return {};
        }
        auto resource = found->second.resource.lock();
        if (!resource) {
            entries_.erase(found);
        }
        return resource;
    }

    void collect(std::vector<Pointer>& retired) {
        Entries candidates;
        for (const auto& [identifier, entry] : entries_) {
            if (entry.retained) {
                candidates.emplace(identifier, entry.retained.get());
            }
        }
        const auto evictions = policy_->selectEvictions(candidates);
        retired.reserve(evictions.size());
        for (const auto& identifier : evictions) {
            const auto found = entries_.find(identifier);
            if (found != entries_.end() && found->second.retained) {
                retired.push_back(std::move(found->second.retained));
                policy_->remove(identifier);
            }
        }
        std::erase_if(entries_, [](const auto& entry) {
            return entry.second.resource.expired();
        });
        // Retired resources are destroyed outside the lock; remove their expired indices next time.
    }

private:
    std::mutex mutex_;
    std::unique_ptr<Policy> policy_;
    std::map<Identifier, Entry> entries_;
};

template <typename T, typename Identifier = std::string>
class WeakCachePolicy final : public Cache<T, Identifier>::Policy {
public:
    void access(const Identifier&, const T&) override {}

    std::vector<Identifier> selectEvictions(const typename Cache<T, Identifier>::Entries& entries) override {
        std::vector<Identifier> result;
        result.reserve(entries.size());
        for (const auto& [identifier, resource] : entries) {
            result.push_back(identifier);
        }
        return result;
    }

    void remove(const Identifier&) noexcept override {}
    void clear() noexcept override {}
};

template <typename T, typename Identifier = std::string>
class LRUCachePolicy final : public Cache<T, Identifier>::Policy {
public:
    explicit LRUCachePolicy(std::size_t capacity) : capacity_(capacity) {}

    void access(const Identifier& identifier, const T&) override {
        const auto found = index_.find(identifier);
        if (found != index_.end()) {
            order_.splice(order_.begin(), order_, found->second);
            return;
        }
        order_.push_front(identifier);
        try {
            index_.emplace(identifier, order_.begin());
        } catch (...) {
            order_.pop_front();
            throw;
        }
    }

    std::vector<Identifier> selectEvictions(const typename Cache<T, Identifier>::Entries&) override {
        std::vector<Identifier> result;
        auto oldest = order_.rbegin();
        for (auto count = order_.size(); count > capacity_; --count) {
            result.push_back(*oldest++);
        }
        return result;
    }

    void remove(const Identifier& identifier) noexcept override {
        const auto found = index_.find(identifier);
        if (found != index_.end()) {
            order_.erase(found->second);
            index_.erase(found);
        }
    }

    void clear() noexcept override {
        index_.clear();
        order_.clear();
    }

private:
    const std::size_t capacity_;
    std::list<Identifier> order_;
    std::map<Identifier, typename decltype(order_)::iterator> index_;
};

template <typename T, typename Identifier = std::string>
class FIFOCachePolicy final : public Cache<T, Identifier>::Policy {
public:
    explicit FIFOCachePolicy(std::size_t capacity) : capacity_(capacity) {}

    void access(const Identifier& identifier, const T&) override {
        if (index_.contains(identifier)) {
            return;
        }
        order_.push_front(identifier);
        try {
            index_.emplace(identifier, order_.begin());
        } catch (...) {
            order_.pop_front();
            throw;
        }
    }

    std::vector<Identifier> selectEvictions(const typename Cache<T, Identifier>::Entries&) override {
        std::vector<Identifier> result;
        auto oldest = order_.rbegin();
        for (auto count = order_.size(); count > capacity_; --count) {
            result.push_back(*oldest++);
        }
        return result;
    }

    void remove(const Identifier& identifier) noexcept override {
        const auto found = index_.find(identifier);
        if (found != index_.end()) {
            order_.erase(found->second);
            index_.erase(found);
        }
    }

    void clear() noexcept override {
        index_.clear();
        order_.clear();
    }

private:
    const std::size_t capacity_;
    std::list<Identifier> order_;
    std::map<Identifier, typename decltype(order_)::iterator> index_;
};

VOLCANO_END

#endif // VOLCANO_CACHE_H
