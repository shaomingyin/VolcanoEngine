//
//
#include <Volcano/Net/Console.h>

VOLCANO_NET_BEGIN

Console::Console()
	: Gui::Window("Console", 100, 100, 400, 300)
	, commit_button_("Commit", this) {
}

VOLCANO_NET_END
