//
//
#ifndef VOLCANO_LAUNCHER_CONSOLE_HPP
#define VOLCANO_LAUNCHER_CONSOLE_HPP

#include <Volcano/Gui/Window.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

class Console: public Gui::Window {
public:
    Console(Gui::Context& context);
};

VOLCANO_LAUNCHER_END

#endif // VOLCANO_LAUNCHER_CONSOLE_HPP
