//
//
#ifndef VOLCANO_GUI_MENUBAR_H
#define VOLCANO_GUI_MENUBAR_H

#include <vector>
#include <string>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Menu.h>
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

class MenuBar: public Object {
public:
	MenuBar(Object* parent = nullptr);
	MenuBar(std::vector<Menu>&& menus, Object* parent = nullptr);

public:
	std::vector<Menu>& menus() {
		return menus_;
	}

	const std::vector<Menu>& menus() const {
		return menus_;
	}

protected:
	void paint() override;

private:
	std::string label_;
	std::vector<Menu> menus_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_MENUBAR_H
