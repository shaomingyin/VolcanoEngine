//
//
#ifndef VOLCANO_INPUT_COMMON_HPP
#define VOLCANO_INPUT_COMMON_HPP

#include <Volcano/Common.hpp>

#define VOLCANO_INPUT_BEGIN VOLCANO_BEGIN namespace Input {
#define VOLCANO_INPUT_END } VOLCANO_END

VOLCANO_INPUT_BEGIN

enum MouseButton {
	MouseButton1 = 0,
	MouseButton2,
	MouseButton3,
	MouseButton4,
	MouseButton5,
	MouseButton6,
	MouseButton7,
	MouseButton8,
};

enum Key {
	KeyEscape = 0,

	KeyF1,
	KeyF2,

	KeyReturn,

	KeyMax,
	KeyUnknown
};

const char *keyToName(Key key);
Key KeyFromName(std::string_view name);

VOLCANO_INPUT_END

#endif // VOLCANO_INPUT_COMMON_HPP
