//
//
#include <Volcano/Input/Common.hpp>

VOLCANO_INPUT_BEGIN

static const char *keyNameMap[KeyMax] = {
	"Escape"
};

const char *keyToName(Key key)
{
	if (0 <= key && key < KeyMax)
		return keyNameMap[key];

	return "Unknown";
}

Key KeyFromName(std::string_view name)
{
	for (int i = 0; i < KeyMax; ++i) {
		if (name == keyNameMap[i])
			return (Key)i;
	}

	return KeyUnknown;
}

VOLCANO_INPUT_END
