//
//
#ifndef VOLCANO_GRAPHICS_SCREEN_H
#define VOLCANO_GRAPHICS_SCREEN_H

#include <imgui_impl_opengl3.h>

#include <Volcano/Gui/Window.h>
#include <Volcano/World/Component.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/FrameBuffer.h>

VOLCANO_GRAPHICS_BEGIN

class Screen: public FrameBuffer {
public:
	Screen(int w, int h);
	virtual ~Screen() = default;

public:
	void resize(int w, int h) override;
	bool beginFrame() override;
	void endFrame() override;

private:
	ImGuiContext* imgui_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SCREEN_H
