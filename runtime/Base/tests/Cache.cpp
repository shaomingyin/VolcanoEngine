#include <array>
#include <barrier>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include <Volcano/Cache.h>

namespace {
using IntCache = Volcano::Cache<int>;
using Volcano::FIFOCachePolicy;
using Volcano::LRUCachePolicy;
using Volcano::WeakCachePolicy;

TEST(BaseCache, WeakPolicyReusesLiveObjectsAndReloadsAfterRelease) {
    IntCache cache(std::make_unique<WeakCachePolicy<int>>());
    int calls = 0;
    auto loader = [&] {
        return std::make_shared<int>(++calls);
    };
    auto first = cache.load("a", loader);
    EXPECT_EQ(cache.load("a", loader), first);
    EXPECT_EQ(calls, 1);
    std::weak_ptr<int> weak = first;
    first.reset();
    EXPECT_TRUE(weak.expired());
    cache.gc();
    EXPECT_EQ(*cache.load("a", loader), 2);
}

TEST(BaseCache, LruEvictsLeastRecentlyAccessedRetention) {
    IntCache cache(std::make_unique<LRUCachePolicy<int>>(2));
    auto loader = [] {
        return std::make_shared<int>(1);
    };
    std::weak_ptr<int> a = cache.load("a", loader);
    std::weak_ptr<int> b = cache.load("b", loader);
    cache.load("a", loader);
    std::weak_ptr<int> c = cache.load("c", loader);
    EXPECT_FALSE(a.expired());
    EXPECT_TRUE(b.expired());
    EXPECT_FALSE(c.expired());
    cache.clear();
    EXPECT_TRUE(a.expired());
    EXPECT_TRUE(c.expired());
}

TEST(BaseCache, FifoEvictsOldestRetentionRegardlessOfAccess) {
    IntCache cache(std::make_unique<FIFOCachePolicy<int>>(2));
    auto loader = [] {
        return std::make_shared<int>(1);
    };
    std::weak_ptr<int> a = cache.load("a", loader);
    std::weak_ptr<int> b = cache.load("b", loader);
    cache.load("a", loader);
    std::weak_ptr<int> c = cache.load("c", loader);
    EXPECT_TRUE(a.expired());
    EXPECT_FALSE(b.expired());
    EXPECT_FALSE(c.expired());
}

TEST(BaseCache, EvictionKeepsExternallyOwnedObjectsDiscoverable) {
    IntCache cache(std::make_unique<LRUCachePolicy<int>>(1));
    int calls = 0;
    auto loader = [&] {
        return std::make_shared<int>(++calls);
    };
    auto a = cache.load("a", loader);
    cache.load("b", loader);
    EXPECT_EQ(cache.load("a", loader), a);
    EXPECT_EQ(calls, 2);
    cache.clear();
    EXPECT_EQ(*a, 1);
    EXPECT_NE(cache.load("a", loader), a);
}

TEST(BaseCache, ZeroCapacityRetainsNothingAndStillReusesExternalHandles) {
    IntCache lru(std::make_unique<LRUCachePolicy<int>>(0));
    IntCache fifo(std::make_unique<FIFOCachePolicy<int>>(0));
    auto loader = [] {
        return std::make_shared<int>(1);
    };
    for (auto* cache : {&lru, &fifo}) {
        auto resource = cache->load("a", loader);
        EXPECT_EQ(cache->load("a", loader), resource);
        std::weak_ptr<int> weak = resource;
        resource.reset();
        EXPECT_TRUE(weak.expired());
    }
}

TEST(BaseCache, LoaderFailuresAreNotCachedAndAllowRetry) {
    EXPECT_THROW(IntCache(nullptr), std::invalid_argument);
    IntCache cache(std::make_unique<WeakCachePolicy<int>>());
    EXPECT_THROW(cache.load("a",
                            []() -> std::shared_ptr<int> {
                                throw std::runtime_error("Read failed");
                            }),
                 std::runtime_error);
    EXPECT_THROW(cache.load("a",
                            [] {
                                return std::shared_ptr<int>{};
                            }),
                 std::runtime_error);
    EXPECT_EQ(*cache.load("a",
                          [] {
                              return std::make_shared<int>(42);
                          }),
              42);
}

TEST(BaseCache, LoaderRunsOutsideTheLockAndCanLoadDependencies) {
    IntCache cache(std::make_unique<WeakCachePolicy<int>>());
    std::shared_ptr<int> dependency;
    auto result = cache.load("outer", [&] {
        dependency = cache.load("inner", [] {
            return std::make_shared<int>(7);
        });
        return std::make_shared<int>(*dependency + 1);
    });
    EXPECT_EQ(*result, 8);
}

TEST(BaseCache, ConcurrentMissesPublishOneLiveObject) {
    IntCache cache(std::make_unique<WeakCachePolicy<int>>());
    constexpr std::size_t count = 4;
    std::barrier rendezvous(static_cast<std::ptrdiff_t>(count));
    std::array<std::shared_ptr<int>, count> results;
    {
        std::vector<std::jthread> threads;
        for (std::size_t i = 0; i < count; ++i) {
            threads.emplace_back([&, i] {
                results[i] = cache.load("a", [&] {
                    rendezvous.arrive_and_wait();
                    return std::make_shared<int>(static_cast<int>(i));
                });
            });
        }
    }
    for (const auto& result : results) {
        EXPECT_EQ(result, results[0]);
    }
}

class CustomPolicy final : public IntCache::Policy {
public:
    void access(const std::string&, const int&) override {
        if (fail) {
            throw std::runtime_error("Policy failed");
        }
        ++accesses;
    }

    std::vector<std::string> selectEvictions(const IntCache::Entries& entries) override {
        if (failSelection) {
            throw std::runtime_error("Selection failed");
        }
        std::vector<std::string> result;
        for (const auto& [identifier, resource] : entries) {
            if (*resource > maximum) {
                result.push_back(identifier);
            }
        }
        return result;
    }

    void remove(const std::string&) noexcept override {
        ++removals;
    }

    void clear() noexcept override {
        accesses = 0;
    }

public:
    bool fail = false;
    int accesses = 0;
    bool failSelection = false;
    int maximum = 100;
    int removals = 0;
};

TEST(BaseCache, CustomPolicyControlsRetentionAndFailedPublicationIsRolledBack) {
    auto policy = std::make_unique<CustomPolicy>();
    auto* observed = policy.get();
    IntCache cache(std::move(policy));
    int calls = 0;
    auto loader = [&] {
        return std::make_shared<int>(++calls);
    };
    observed->fail = true;
    EXPECT_THROW(cache.load("a", loader), std::runtime_error);
    observed->fail = false;
    std::weak_ptr<int> resource = cache.load("a", loader);
    EXPECT_EQ(*resource.lock(), 2);
    cache.load("a", loader);
    EXPECT_EQ(observed->accesses, 2);
    cache.clear();
    EXPECT_TRUE(resource.expired());
    EXPECT_EQ(observed->accesses, 0);
}
TEST(BaseCache, GcReevaluatesPolicyWithoutLoadingAndPreservesExternalHandles) {
    auto policy = std::make_unique<CustomPolicy>();
    auto* observed = policy.get();
    IntCache cache(std::move(policy));
    auto loader = [] {
        return std::make_shared<int>(10);
    };
    auto external = cache.load("external", loader);
    std::weak_ptr<int> retained = cache.load("retained", loader);
    EXPECT_FALSE(retained.expired());

    observed->maximum = 5;
    cache.gc();
    EXPECT_TRUE(retained.expired());
    EXPECT_EQ(observed->removals, 2);
    EXPECT_EQ(cache.load("external", loader), external);
    observed->maximum = 100;
    cache.load("external", loader);
    std::weak_ptr<int> weak = external;
    external.reset();
    EXPECT_FALSE(weak.expired());
    cache.clear();
    EXPECT_TRUE(weak.expired());
}

TEST(BaseCache, SelectionFailureKeepsResourcesAndCanBeRetried) {
    auto policy = std::make_unique<CustomPolicy>();
    auto* observed = policy.get();
    IntCache cache(std::move(policy));
    std::weak_ptr<int> weak = cache.load("a", [] {
        return std::make_shared<int>(10);
    });
    observed->maximum = 0;
    observed->failSelection = true;
    EXPECT_THROW(cache.gc(), std::runtime_error);
    EXPECT_FALSE(weak.expired());
    observed->failSelection = false;
    cache.gc();
    EXPECT_TRUE(weak.expired());
}

TEST(BaseCache, ResourceDestructionRunsOutsideLockForEvictionAndClear) {
    IntCache cache(std::make_unique<LRUCachePolicy<int>>(1));
    int destroyed = 0;
    auto loader = [&] {
        return std::shared_ptr<int>(new int(1), [&](int* value) {
            cache.gc();
            ++destroyed;
            delete value;
        });
    };
    cache.load("a", loader);
    cache.load("b", loader);
    EXPECT_EQ(destroyed, 1);
    cache.clear();
    EXPECT_EQ(destroyed, 2);
}
} // namespace
