//
//
#ifndef VOLCANO_SCOPEGUARD_HPP
#define VOLCANO_SCOPEGUARD_HPP

#include <functional>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class ScopeGuard final {
public:
	using Function = std::function<void(void)>;

public:
	ScopeGuard(Function &fn);
	ScopeGuard(Function &&fn);
	~ScopeGuard(void);

public:
	void dismiss(void);

private:
	Function m_fn;
	bool m_dismissed;
};

VOLCANO_INLINE ScopeGuard::ScopeGuard(Function &fn):
	m_fn(fn),
	m_dismissed(false)
{
}

VOLCANO_INLINE ScopeGuard::ScopeGuard(Function &&fn):
	m_fn(std::move(fn)),
	m_dismissed(false)
{
}

VOLCANO_INLINE ScopeGuard::~ScopeGuard(void)
{
	if (m_fn && !m_dismissed)
		m_fn();
}

VOLCANO_INLINE void ScopeGuard::dismiss(void)
{
	m_dismissed = true;
}

VOLCANO_END

#endif // VOLCANO_SCOPEGUARD_HPP
