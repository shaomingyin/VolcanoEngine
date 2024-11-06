//
//
#ifndef VOLCANO_GRAPHICS_SCREEN_H
#define VOLCANO_GRAPHICS_SCREEN_H

#include <imgui_impl_opengl3.h>

#include <Volcano/Gui/Window.h>
#include <Volcano/World/Component.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Screen: public World::Movable {
public:
	Screen(int w = 800, int h = 600);
	virtual ~Screen() = default;

public:
	void resize(int w, int h);

	int width() const {
		return width_;
	}

	int height() const {
		return height_;
	}

	void beginFrame() {
		ImGui::SetCurrentContext(imgui_);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void endFrame() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

private:
	ImGuiContext* imgui_;
	int width_;
	int height_;
	// TODO frame buffer
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SCREEN_H
