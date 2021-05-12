//
//
#ifndef VOLCANO_INPUT_CONTEXT_HPP
#define VOLCANO_INPUT_CONTEXT_HPP

#include <queue>
#include <functional>

#include <sigslot/signal.hpp>

#include <Volcano/SpinLock.hpp>
#include <Volcano/Input/Common.hpp>

VOLCANO_INPUT_BEGIN

class Context {
	VOLCANO_DISABLE_COPY_AND_MOVE(Context)

public:
	enum {
		SourceKeyBoard = 0x1,
		SourceMouse = 0x2,
		SourceGamepad = 0x4,
	};

public:
	Context(void);
	virtual ~Context(void);

public:
	void clear(void);
	void commit(void);
	int sourceMask(void) const;
	void setSourceMask(int mask);
	float mouseSensitivity(void) const;
	void setMouseSensitivity(float v);

public:
	void feedKeyDown(Key key);
	void feedKeyUp(Key key);
	void feedMouseButtonDown(MouseButton mb);
	void feedMouseButtonUp(MouseButton mb);
	void feedMouseWheel(int value);
	void feedMouseMove(int dx, int dy);

public:
	sigslot::signal<Key> onKeyDown;
	sigslot::signal<Key> onKeyUp;
	sigslot::signal<MouseButton> onMouseButtonDown;
	sigslot::signal<MouseButton> onMouseButtonUp;
	sigslot::signal<int> onMouseWheel;
	sigslot::signal<float, float> onMouseMove;

private:
	using Event = std::function<void (void)>;
	using EventQueue = std::queue<Event>;
	
	SpinLock m_spinLock;
	int m_sourceMask;
	float m_mouseSensitivity;
	EventQueue m_eventQueue;
};

VOLCANO_INPUT_END

#endif // VOLCANO_INPUT_CONTEXT_HPP
