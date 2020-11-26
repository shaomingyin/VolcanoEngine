//
//
#ifndef VOLCANO_SCOPE_GUARD_HPP
#define VOLCANO_SCOPE_GUARD_HPP

#include <utility>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

template<class FN>
class ScopeGuard: public Noncopyable {
public:
    inline ScopeGuard(FN fn):
        m_fn(std::move(fn)),
        m_active(true)
    {
    }

    inline ScopeGuard(ScopeGuard &&that):
        m_fn(std::move(that.m_fn)),
        m_active(that.m_active)
    {
        that.dismiss();
    }

    inline ~ScopeGuard(void)
    {
        if (m_active)
            m_fn();
    }

public:
    inline void dismiss(void)
    {
        m_active = false;
    }

private:
    FN m_fn;
    bool m_active;
};

template<class FN>
inline ScopeGuard<FN> scopeGuard(FN fn)
{
    return ScopeGuard<FN>(std::move(fn));
}

namespace Detail {
    enum class ScopeGuardOnExit { };
    template<typename FN>
    ScopeGuard<FN> operator+(ScopeGuardOnExit, FN &&fn)
    {
        return ScopeGuard<FN>(std::forward<FN>(fn));
    }
}

VOLCANO_END

#define VOLCANO_SCOPE_EXIT(name) \
    auto name = Volcano::Detail::ScopeGuardOnExit() + [&](void) -> void

#define VOLCANO_SCOPE_TRACE(name) \
    VOLCANO_LOGD("Trace '%s' start.", VOLCANO_STRIZE(name)); \
    VOLCANO_SCOPE_EXIT(VOLCANO_AUTONAME(name)) { VOLCANO_LOGD("Trace '%s' exit.", VOLCANO_STRIZE(name)); }

#endif // SCOPE_GUARD_HPP
