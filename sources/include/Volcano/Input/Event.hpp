//
//
#ifndef VOLCANO_INPUT_EVENT_HPP
#define VOLCANO_INPUT_EVENT_HPP

#include <string>
#include <chrono>

#include <Volcano/Input/Common.hpp>

VOLCANO_INPUT_BEGIN

struct Event {
    enum Type {
        TypeInvalid = 0,
        TypeKeyDown,
        TypeKeyUp,
        TypeMouseMove,
        TypeMouseButton
    };

    Type type;
    std::chrono::system_clock::time_point timePoint;

    union {
        struct {
            int keyCode;
        } keyDown;
        struct {
            int keyCode;
        } keyUp;
        struct {
            int dx;
            int dy;
        } mouseMove;
        struct {
            int button;
        } mouseButton;
    } params;
};

VOLCANO_INPUT_END

#endif // VOLCANO_INPUT_EVENT_HPP
