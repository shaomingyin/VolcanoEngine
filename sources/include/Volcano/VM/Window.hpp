//
//
#ifndef VOLCANO_VM_WINDOW_HPP
#define VOLCANO_VM_WINDOW_HPP

#include <string>
#include <string_view>

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Renderer.hpp>

VOLCANO_VM_BEGIN

class Window {
public:
	Window(void) = default;
	virtual ~Window(void) = default;

public:
	virtual void update(void) = 0;
	virtual bool isVisible(void) const = 0;
	virtual void setVisible(bool v) = 0;
	virtual const Eigen::Vector2i &size(void) const = 0;
	virtual void setSize(const Eigen::Vector2i &size) = 0;
	virtual bool isFullscreen(void) const = 0;
	virtual void setFullscreen(bool v) = 0;
	virtual const std::string &title(void) const = 0;
	virtual void setTitle(std::string_view title) = 0;
	virtual Renderer *renderer(void) = 0;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_WINDOW_HPP
