//
//
#ifndef VOLCANO_SCOPEGUARD_H
#define VOLCANO_SCOPEGUARD_H

#include <utility>

#include <Volcano/Common.h>

VOLCANO_BEGIN

template <typename FN>
class ScopeGuard final {
public:
    explicit ScopeGuard(FN&& fn) noexcept
        : fn_(std::move(fn)) {
    }

    explicit ScopeGuard(const FN& fn) noexcept
        : fn_(fn) {
    }

    ScopeGuard(const ScopeGuard&) = delete;

    ScopeGuard(ScopeGuard &&other) noexcept
        : fn_(std::move(other.fn_))
        , dismissed_(other.dismissed_) {
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

    ScopeGuard& operator=(ScopeGuard&& other) noexcept {
        if (this != &other) {
            fn_ = std::move(other.fn_);
            dismissed_ = other.dismissed_;
            other.dismissed_ = true;
        }
        return (*this);
    }

 public:
    void dismiss() noexcept {
        dismissed_ = true;
    }

private:
    FN fn_;
    bool dismissed_ = false;
};

template <typename FN>
ScopeGuard<typename std::decay<FN>::type> scopeGuard(FN&& fn) noexcept {
    return ScopeGuard<typename std::decay<FN>::type>(std::forward<FN>(fn));
}

VOLCAN_END

#endif  // VOLCANO_SCOPEGUARD_H
