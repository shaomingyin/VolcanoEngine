#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include <Volcano/ScopeGuard.h>

namespace {

struct CopyableCleanup {
public:
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

public:
    int& calls;
    bool& fail;
};

TEST(BaseUtility, ScopeGuard) {
    int calls = 0;
    {
        auto source = Volcano::scopeGuard([&] {
            ++calls;
        });
        auto moved = std::move(source);
    }

    EXPECT_TRUE((calls == 1));
    {
        auto guard = Volcano::scopeGuard([&] {
            ++calls;
        });
        guard.dismiss();
        auto moved = std::move(guard);
    }

    EXPECT_TRUE((calls == 1));
    try {
        auto guard = Volcano::scopeGuard([&] {
            ++calls;
        });
        throw std::runtime_error("Unwind");
    } catch (const std::runtime_error&) {
    }

    EXPECT_TRUE((calls == 2));
    {
        auto guard = Volcano::scopeGuard([] {
            throw std::runtime_error("Cleanup");
        });
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
        try {
            auto moved = std::move(source);
        } catch (const std::runtime_error&) {
            caught = true;
        }

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
