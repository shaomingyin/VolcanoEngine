//
//
#ifndef VOLCANO_SYSTEM_CONSOLE_H
#define VOLCANO_SYSTEM_CONSOLE_H

#include <Volcano/Gui/Window.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Console: public Gui::Window {
public:
	Console(NVGcontext* nvg);
	virtual ~Console() = default;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CONSOLE_H
