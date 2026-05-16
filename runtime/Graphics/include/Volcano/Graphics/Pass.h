//
//
#ifndef VOLCANO_GRAPHICS_PASS_H
#define VOLCANO_GRAPHICS_PASS_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Context.h>
#include <Volcano/Graphics/Pipeline.h>
#include <Volcano/Graphics/Target.h>
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

class Pass {
public:
	Pass(Context& context);
	virtual ~Pass() = default;

public:
	Context& context() noexcept {
		return context_;
	}

	virtual void draw(const View& view, Target& target) noexcept = 0;

private:
	Context& context_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PASS_H
