//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/Screen.h>

VOLCANO_GRAPHICS_BEGIN

Screen::Screen(int w, int h)
	: FrameBuffer(w, h)
	, imgui_(ImGui::CreateContext()) {
	if (imgui_ == nullptr) {
		throw Error(Errc::InvalidContext);
	}
}

void Screen::resize(int w, int h) {
	FrameBuffer::resize(w, h);
}

bool Screen::beginFrame() {
	if (!FrameBuffer::beginFrame()) {
		return false;
	}

	ImGui::SetCurrentContext(imgui_);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	return true;
}

void Screen::endFrame() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	FrameBuffer::endFrame();
}

VOLCANO_GRAPHICS_END
