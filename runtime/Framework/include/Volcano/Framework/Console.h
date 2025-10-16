//
//
#ifndef VOLCANO_FRAMEWORK_CONSOLE_H
#define VOLCANO_FRAMEWORK_CONSOLE_H

#include <Volcano/Gui/Window.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

class Console: public Gui::Window {
public:
    Console(Gui::Context& context);
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_CONSOLE_H
