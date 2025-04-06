//
//
#ifndef VOLCANO_SCOPEGUARD_H
#define VOLCANO_SCOPEGUARD_H

#include <type_traits>
#include <utility>

#include <Volcano/Common.h>

VOLCANO_BEGIN

template <typename F>
class ScopeGuard {
public:
    explicit ScopeGuard(F&& f) noexcept
        : func_(std::move(f)) {
    }

    explicit ScopeGuard(const F& f) noexcept
        : func_(f) {
    }

    ScopeGuard(ScopeGuard&& other) noexcept
        : func_(std::move(other.func_))
        , invoke_(std::exchange(other.invoke_, false)) {
    }

    ~ScopeGuard() noexcept {
        if (invoke_) {
            func_();
        }
    }

    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

public:
    void dismiss() noexcept {
        invoke_ = false;
    }

private:
    F func_;
    bool invoke_ = true;
};

template <typename F> ScopeGuard(F(&)()) -> ScopeGuard<F(*)()>;

template <typename F>
[[nodiscard]] ScopeGuard<typename std::decay<F>::type> scopeGuard(F&& f) {
    return ScopeGuard<typename std::decay<F>::type>(std::forward<F>(f));
}

VOLCANO_END

#endif // VOLCANO_SCOPEGUARD_H
