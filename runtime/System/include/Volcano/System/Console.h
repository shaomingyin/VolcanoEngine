//
//
#ifndef VOLCANO_SYSTEM_CONSOLE_H
#define VOLCANO_SYSTEM_CONSOLE_H

#include <Volcano/Gui/Window.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Console: public Gui::Window {
public:
	Console(Gui::Rect rect);
	virtual ~Console() = default;

public:
	void update() override;

	void toggle() {
		toggle_ = !toggle_;
	}

private:
	bool toggle_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_CONSOLE_H
