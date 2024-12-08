//
//
#include <Volcano/System/Console.h>

VOLCANO_SYSTEM_BEGIN

Console::Console()
	: Gui::Window("Console", 100, 100, 400, 300)
	, commit_button_("Commit", this) {
}

VOLCANO_SYSTEM_END
