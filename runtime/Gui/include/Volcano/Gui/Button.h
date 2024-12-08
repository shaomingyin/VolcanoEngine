//
//
#ifndef VOLCANO_GUI_BUTTON_H
#define VOLCANO_GUI_BUTTON_H

#include <string>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

class Button: public Object {
public:
	Button(std::string label, Object* parent = nullptr);
	~Button() override = default;

protected:
	void paint() override;

private:
	std::string label_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_BUTTON_H
