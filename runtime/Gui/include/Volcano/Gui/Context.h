//
//
#ifndef VOLCANO_GUI_CONTEXT_H
#define VOLCANO_GUI_CONTEXT_H

#include <string>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Style.h>
#include <Volcano/Gui/Cache.h>

VOLCANO_GUI_BEGIN

class Context {
public:
	Context() = default;
	virtual ~Context() = default;

public:
	FontCache::Pointer getFont(const std::string& family) noexcept {
		return getFontCache().get(family);
	}

	FontCache::Pointer getFont(const Style& style) noexcept {
		return getFont(style.font_family);
	}

	virtual FontCache& getFontCache() noexcept = 0;
	virtual const Style& getDefaultStyle() const noexcept = 0;
	virtual const Style& getWindowTitleStyle() const noexcept = 0;
	virtual float getWindowTitleHeight() const noexcept = 0;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_CONTEXT_H
