//
//
#include <mutex>

#include <Volcano/Input/Context.hpp>

VOLCANO_INPUT_BEGIN

Context::Context(void):
	m_sourceMask(SourceKeyBoard | SourceMouse | SourceGamepad),
	m_mouseSensitivity(1.0f)
{
}

Context::~Context(void)
{
}

void Context::clear(void)
{
	std::unique_lock<SpinLock> locker(m_spinLock);

	while (!m_eventQueue.empty())
		m_eventQueue.pop();
}

void Context::commit(void)
{
	std::unique_lock<SpinLock> locker(m_spinLock);

	while (!m_eventQueue.empty()) {
		m_eventQueue.front()();
		m_eventQueue.pop();
	}
}

int Context::sourceMask(void) const
{
	return m_sourceMask;
}

void Context::setSourceMask(int mask)
{
	std::unique_lock<SpinLock> locker(m_spinLock);

	m_sourceMask = mask;
}

float Context::mouseSensitivity(void) const
{
	return m_mouseSensitivity;
}

void Context::setMouseSensitivity(float v)
{
	std::unique_lock<SpinLock> locker(m_spinLock);

	if (v > 0.0f)
		m_mouseSensitivity = v;
}

void Context::feedKeyDown(Key key)
{
	std::unique_lock<SpinLock> locker(m_spinLock);

	if (m_sourceMask & SourceKeyBoard) {
		m_eventQueue.push([this, key] {
			onKeyDown(key);
		});
	}
}

void Context::feedKeyUp(Key key)
{
	std::unique_lock<SpinLock> locker(m_spinLock);

	if (m_sourceMask & SourceKeyBoard) {
		m_eventQueue.push([this, key] {
			onKeyUp(key);
		});
	}
}

void Context::feedMouseButtonDown(MouseButton mb)
{
	std::unique_lock<SpinLock> locker(m_spinLock);

	if (m_sourceMask & SourceMouse) {
		m_eventQueue.push([this, mb] {
			onMouseButtonDown(mb);
		});
	}
}

void Context::feedMouseButtonUp(MouseButton mb)
{
	std::unique_lock<SpinLock> locker(m_spinLock);

	if (m_sourceMask & SourceMouse) {
		m_eventQueue.push([this, mb] {
			onMouseButtonUp(mb);
		});
	}
}

void Context::feedMouseWheel(int value)
{
	if (value == 0)
		return;

	std::unique_lock<SpinLock> locker(m_spinLock);

	if (m_sourceMask & SourceMouse) {
		m_eventQueue.push([this, value] {
			onMouseWheel(value);
		});
	}
}

void Context::feedMouseMove(int dx, int dy)
{
	if (dx == 0 && dy == 0)
		return;

	std::unique_lock<SpinLock> locker(m_spinLock);

	if (m_sourceMask & SourceMouse) {
		float realDx = dx;
		float realDy = dy;

		realDx *= m_mouseSensitivity;
		realDy *= m_mouseSensitivity;

		m_eventQueue.push([this, realDx, realDy] {
			onMouseMove(realDx, realDy);
		});
	}
}

VOLCANO_INPUT_END
