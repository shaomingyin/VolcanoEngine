#include <gtest/gtest.h>
#include <stdexcept>
#include <type_traits>

#include <Volcano/Cache.h>
#include <Volcano/ScopeGuard.h>

namespace {

class IntCache final : public Volcano::Cache<int> {
public:
    using Cache::Cache;
    int loads = 0;

private:
    Pointer load(const std::string& key) override {
        ++loads;
        if (key == "missing") {
            return nullptr;
        }
        if (key == "error") {
            throw std::runtime_error("Load failed");
        }
        return std::make_shared<int>(loads);
    }
};

TEST(BaseUtility, Cache) {
    IntCache cache(2);
    const auto a = cache.get("a");
    const auto b = cache.get("b");
    EXPECT_TRUE((cache.get("a") == a && cache.loads == 2));
    cache.get("c");
    EXPECT_TRUE((cache.get("a") == a && cache.loads == 3));
    EXPECT_TRUE((cache.get("b") != b && cache.loads == 4));
    cache.clear();
    EXPECT_TRUE((cache.get("a") != a && cache.loads == 5));

    IntCache failures(1);
    const auto retained = failures.get("a");
    EXPECT_TRUE((!failures.get("missing") && !failures.get("missing")));
    EXPECT_TRUE((failures.loads == 3));
    bool caught = false;
    try { failures.get("error"); }
    catch (const std::runtime_error&) { caught = true; }
    EXPECT_TRUE((caught && failures.get("a") == retained));

    IntCache disabled(0);
    const auto first = disabled.get("a");
    EXPECT_TRUE((disabled.get("a") != first && disabled.loads == 2));
    IntCache moved(std::move(cache));
    const auto value = moved.get("a");
    IntCache assigned(0);
    assigned = std::move(moved);
    EXPECT_TRUE((assigned.get("a") == value));
}

struct CopyableCleanup {
    int& calls;
    bool& fail;

    CopyableCleanup(int& calls, bool& fail) : calls(calls), fail(fail) {
    }

    CopyableCleanup(const CopyableCleanup& other) : calls(other.calls), fail(other.fail) {
        if (fail) {
            throw std::runtime_error("Copy failed");
        }
    }

    CopyableCleanup(CopyableCleanup&& other) noexcept(false) : calls(other.calls), fail(other.fail) {
        throw std::runtime_error("Move must use the copy fallback");
    }

    void operator()() const noexcept {
        ++calls;
    }
};

TEST(BaseUtility, ScopeGuard) {
    int calls = 0;
    {
        auto source = Volcano::scopeGuard([&] { ++calls; });
        auto moved = std::move(source);
    }
    EXPECT_TRUE((calls == 1));
    {
        auto guard = Volcano::scopeGuard([&] { ++calls; });
        guard.dismiss();
        auto moved = std::move(guard);
    }
    EXPECT_TRUE((calls == 1));
    try {
        auto guard = Volcano::scopeGuard([&] { ++calls; });
        throw std::runtime_error("Unwind");
    } catch (const std::runtime_error&) {
    }
    EXPECT_TRUE((calls == 2));
    {
        auto guard = Volcano::scopeGuard([] { throw std::runtime_error("Cleanup"); });
    }

    {
        auto source = Volcano::scopeGuard([payload = std::make_unique<int>(3), &calls] {
            calls += *payload;
        });
        static_assert(!std::is_move_assignable_v<decltype(source)>);
        auto moved = std::move(source);
    }
    EXPECT_TRUE((calls == 5));

    bool fail = false;
    const CopyableCleanup cleanup(calls, fail);
    static_assert(!noexcept(Volcano::scopeGuard(cleanup)));
    {
        auto source = Volcano::scopeGuard(cleanup);
        static_assert(!std::is_nothrow_move_constructible_v<decltype(source)>);
        fail = true;
        bool caught = false;
        try { auto moved = std::move(source); }
        catch (const std::runtime_error&) { caught = true; }
        EXPECT_TRUE((caught));
    }
    EXPECT_TRUE((calls == 6));
    fail = false;
    {
        auto source = Volcano::scopeGuard(cleanup);
        auto moved = std::move(source);
    }
    EXPECT_TRUE((calls == 7));
}

} // namespace
