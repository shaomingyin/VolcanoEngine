//
//
#ifndef VOLCANO_GUI_CONTEXT_H
#define VOLCANO_GUI_CONTEXT_H

#include <vector>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

class Context {
public:
	class WidgetBase {
	public:
		WidgetBase() = default;
		virtual ~WidgetBase() = default;

	public:
		virtual void handleEvent(const SDL_Event& evt) = 0;
	};

public:
	Context() = default;
	virtual ~Context() = default;

public:
	virtual NVGcontext* nvg() = 0;
	virtual void addWidget(WidgetBase* p);
	virtual void removeWidget(WidgetBase* p);

protected:
	using Widgets = std::vector<WidgetBase*>;

	Widgets& widgets() {
		return widgets_;
	}

private:
	Widgets widgets_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_CONTEXT_H
