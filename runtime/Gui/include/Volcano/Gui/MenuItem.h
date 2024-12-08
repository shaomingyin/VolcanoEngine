//
//
#ifndef VOLCANO_GUI_MENUITEM_H
#define VOLCANO_GUI_MENUITEM_H

#include <string>
#include <functional>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

class MenuItem: public Object {
public:
	MenuItem(std::string&& label, bool* selected = nullptr, Object* parent = nullptr);
	MenuItem(const MenuItem&) = default;
	MenuItem(MenuItem&&) = default;
	MenuItem& operator=(const MenuItem&) = default;
	MenuItem& operator=(MenuItem&&) = default;

public:
	const std::string& label() const {
		return label_;
	}

	void setLabel(std::string v) {
		if (label_ != v) {
			label_ = std::move(v);
		}
	}

	bool isCheckable() const {
		return (selected_ != nullptr);
	}

	void setCheckable(bool* p) {
		selected_ = p;
	}

protected:
	void paint() override;

private:
	std::string label_;
	bool* selected_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_MENUITEM_H
