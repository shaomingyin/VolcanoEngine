//
//
#include <Volcano/Gui/Window.h>

VOLCANO_GUI_BEGIN

Window::Window(std::string title, float x, float y, float w, float h, int flags)
	: title_(std::move(title))
	, flags_(flags)
	, position_(x, y)
	, size_(w, h)
	, menu_bar_(this) {
}

void Window::paint() {
	if (state_ & StatePositionDirty) {
		ImGui::SetNextWindowPos({ position().x(), position().y() });
		state_ &= ~StatePositionDirty;
	}

	if (state_ & StateSizeDirty) {
		ImGui::SetNextWindowSize({ size().x(), size().y() });
		state_ &= ~StateSizeDirty;
	}

	ImGui::Begin(title().c_str(), visiblility(), flags_);
	updateChidlren();
	ImGui::End();

	auto posistion = ImGui::GetWindowPos();
	auto size = ImGui::GetWindowSize();
	position_ = Eigen::Vector2f(posistion.x, posistion.y);
	size_ = Eigen::Vector2f(size.x, size.y);
}

VOLCANO_GUI_END
