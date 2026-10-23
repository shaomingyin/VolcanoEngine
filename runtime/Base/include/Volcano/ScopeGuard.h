//
//
#ifndef VOLCANO_SCOPEGUARD_H
#define VOLCANO_SCOPEGUARD_H

#include <utility>
#include <type_traits>

#include <Volcano/Common.h>

VOLCANO_BEGIN

template <typename FN>
class ScopeGuard final {
public:
    explicit ScopeGuard(FN&& fn) noexcept(std::is_nothrow_move_constructible_v<FN>)
        : fn_(std::move(fn)) {
    }

    explicit ScopeGuard(const FN& fn) noexcept(std::is_nothrow_copy_constructible_v<FN>)
        : fn_(fn) {
    }

    ScopeGuard(const ScopeGuard&) = delete;

    ScopeGuard(ScopeGuard &&other) noexcept(
        std::is_nothrow_move_constructible_v<FN> || std::is_nothrow_copy_constructible_v<FN>)
        : fn_(std::move_if_noexcept(other.fn_))
        , dismissed_(other.dismissed_) {
        other.dismissed_ = true;
    }

    ~ScopeGuard() noexcept {
        if (!dismissed_) {
            try{
                fn_();
            } catch (...) {
            }
        }
    }

    ScopeGuard& operator=(const ScopeGuard&) = delete;

    // Assignment would discard the cleanup already owned by the destination.
    ScopeGuard& operator=(ScopeGuard&&) = delete;

 public:
    void dismiss() noexcept {
        dismissed_ = true;
    }

private:
    FN fn_;
    bool dismissed_ = false;
};

template <typename FN>
ScopeGuard<typename std::decay<FN>::type> scopeGuard(FN&& fn) noexcept(
    std::is_nothrow_constructible_v<std::decay_t<FN>, FN&&>) {
    return ScopeGuard<typename std::decay<FN>::type>(std::forward<FN>(fn));
}

VOLCANO_END

#endif  // VOLCANO_SCOPEGUARD_H
