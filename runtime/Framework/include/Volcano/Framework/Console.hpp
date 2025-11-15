//
//
#ifndef VOLCANO_FRAMEWORK_CONSOLE_HPP
#define VOLCANO_FRAMEWORK_CONSOLE_HPP

#include <Volcano/Gui/Window.hpp>
#include <Volcano/Framework/Common.hpp>

VOLCANO_FRAMEWORK_BEGIN

class Console: public Gui::Window {
public:
    Console(Gui::Context& context);
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CONSOLE_HPP
