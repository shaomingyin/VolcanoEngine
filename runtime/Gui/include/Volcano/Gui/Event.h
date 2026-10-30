#ifndef VOLCANO_GUI_EVENT_H
#define VOLCANO_GUI_EVENT_H

#include <cstddef>
#include <cstdint>
#include <string>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

struct Modifiers {
    bool shift = false;
    bool control = false;
    bool alt = false;
    bool super = false;
};

struct PointerEvent {
    enum class Action { Move, Down, Up, Cancel, Enter, Leave };
    enum class Device { Mouse, Touch, Pen };
    enum class Button { None, Primary, Secondary, Middle };

    Action action = Action::Move;
    float x = 0;
    float y = 0;
    Button button = Button::Primary;
    std::uint64_t id = 0;
    Device device = Device::Mouse;
    Modifiers modifiers;
    // Set by Document for the receiving widget, including during capture.
    bool inside = false;
};

struct ScrollEvent {
    enum class Unit { Lines, Pixels };

    float x = 0;
    float y = 0;
    // Positive deltas scroll right/down; the host normalizes native units and signs.
    float deltaX = 0;
    float deltaY = 0;
    Unit unit = Unit::Lines;
    Modifiers modifiers;
};

struct KeyEvent {
    enum class Action { Down, Up, Repeat };
    enum class Key {
        Unknown,
        Enter,
        Space,
        Tab,
        Escape,
        Backspace,
        Delete,
        Insert,
        Home,
        End,
        PageUp,
        PageDown,
        Left,
        Right,
        Up,
        Down,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        Num0,
        Num1,
        Num2,
        Num3,
        Num4,
        Num5,
        Num6,
        Num7,
        Num8,
        Num9,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12
    };

    Action action = Action::Down;
    Key key = Key::Unknown;
    Modifiers modifiers;
    // Host scan code; logical keys above remain independent of the window system.
    std::uint32_t scanCode = 0;
};

struct TextInputEvent {
    std::string text; // Committed UTF-8 text, separate from physical key input.
};

struct CompositionEvent {
    enum class Action { Start, Update, Commit, Cancel };

    Action action = Action::Update;
    std::string text;
    // UTF-8 byte offsets into text; boundaries must not split a code point.
    std::size_t selectionStart = 0;
    std::size_t selectionLength = 0;
};

struct FocusEvent {
    bool focused = false;
};

VOLCANO_GUI_END

#endif
