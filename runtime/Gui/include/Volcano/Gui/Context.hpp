//
//
#ifndef VOLCANO_GUI_CONTEXT_H
#define VOLCANO_GUI_CONTEXT_H

#include <string>

#include <SFML/Graphics/Font.hpp>

#include <Volcano/Cache.hpp>
#include <Volcano/Gui/Common.hpp>
#include <Volcano/Gui/Style.hpp>

VOLCANO_GUI_BEGIN

using FontCache = Cache<sf::Font>;

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
