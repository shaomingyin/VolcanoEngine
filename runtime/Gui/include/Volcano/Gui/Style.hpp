//
//
#ifndef VOLCANO_GUI_STYLE_H
#define VOLCANO_GUI_STYLE_H

#include <string>

#include <SFML/Graphics/Color.hpp>

#include <Volcano/Gui/Common.hpp>

VOLCANO_GUI_BEGIN

struct Style {
	std::string font_family = "Courier New";
	unsigned int font_size = 12;
	sf::Color font_color = sf::Color::Black;
	sf::Color background_color = sf::Color::White;
	sf::Color border_color = sf::Color::Black;
	float border_size = 1.0f;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_STYLE_H
