//
//
#ifndef VOLCANO_GUI_STYLE_H
#define VOLCANO_GUI_STYLE_H

#include <string>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

struct Style {
	std::string font_family = "Courier New";
	unsigned int font_size = 12;
	float border_size = 1.0f;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_STYLE_H
