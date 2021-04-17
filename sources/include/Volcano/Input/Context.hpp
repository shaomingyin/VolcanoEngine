//
//
#ifndef VOLCANO_INPUT_CONTEXT_HPP
#define VOLCANO_INPUT_CONTEXT_HPP

#include <sigslot/signal.hpp>

#include <Volcano/Input/Common.hpp>
#include <Volcano/Input/Event.hpp>

VOLCANO_INPUT_BEGIN

class Context {
public:
    enum Capability {
        HasKeyboard = 0x1,
        HasMouse = 0x2,
        HasGamepad = 0x4,
        HasJokey = 0x8
    };

public:
    Context(void);
    virtual ~Context(void);

public:
    virtual bool init(void);
    virtual void frame(float elapsed);
    virtual int capability(void) = 0;

protected:
    virtual bool pollEvent(Event &out) = 0;
    // hotplug?

private:
    void handleEvent(const Event &event);

public:
    sigslot::signal<int> onKeyEvent;
    sigslot::signal<int, int> onMouseMoveEvent;
    sigslot::signal<int> onMouseButtonEvent;
    // jokey, gamepad?
};

VOLCANO_INPUT_END

#endif // VOLCANO_INPUT_CONTEXT_HPP
