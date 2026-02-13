//
//
#ifndef VOLCANO_LAUNCHER_CONSOLE_H
#define VOLCANO_LAUNCHER_CONSOLE_H

#include <Volcano/Gui/Window.h>
#include <Volcano/Launcher/Common.h>

VOLCANO_LAUNCHER_BEGIN

class Console: public Gui::Window {
public:
    Console(Gui::Context& context);
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_CONSOLE_H
