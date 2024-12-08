//
//
#ifndef VOLCANO_SYSTEM_CONSOLE_H
#define VOLCANO_SYSTEM_CONSOLE_H

#include <Volcano/Gui/Window.h>
#include <Volcano/Gui/Button.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Console: public Gui::Window {
public:
	Console();

public:
	Gui::Button commit_button_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CONSOLE_H
