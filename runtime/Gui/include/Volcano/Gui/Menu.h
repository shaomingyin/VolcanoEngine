//
//
#ifndef VOLCANO_GUI_MENU_H
#define VOLCANO_GUI_MENU_H

#include <vector>
#include <string>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/MenuItem.h>
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

class Menu: public Object {
public:
	Menu(std::string&& label, std::vector<MenuItem>&& items = {}, Object* parent = nullptr);
	Menu(const Menu&) = delete;
	Menu(Menu&&) = default;
	Menu& operator=(const Menu&) = delete;
	Menu& operator=(Menu&&) = default;
	~Menu() override = default;

public:
	std::vector<MenuItem>& items() {
		return items_;
	}

	const std::vector<MenuItem>& items() const {
		return items_;
	}

protected:
	void paint() override;

private:
	std::string label_;
	std::vector<MenuItem> items_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_MENU_H
