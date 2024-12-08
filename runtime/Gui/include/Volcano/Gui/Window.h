//
//
#ifndef VOLCANO_GUI_WINDOW_H
#define VOLCANO_GUI_WINDOW_H

#include <string>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/MenuBar.h>
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

class Window: public Object {
public:
    static constexpr auto FlagNoTitleBar = ImGuiWindowFlags_NoTitleBar;
    static constexpr auto FlagNoResize = ImGuiWindowFlags_NoResize;
    static constexpr auto FlagNoMove = ImGuiWindowFlags_NoMove;
    static constexpr auto FlagNoScrollbar = ImGuiWindowFlags_NoScrollbar;
    static constexpr auto FlagNoScrollWithMouse = ImGuiWindowFlags_NoScrollWithMouse;
    static constexpr auto FlagNoCollapse = ImGuiWindowFlags_NoCollapse;
    static constexpr auto FlagAlwaysAutoResize = ImGuiWindowFlags_AlwaysAutoResize;
    static constexpr auto FlagNoBackground = ImGuiWindowFlags_NoBackground;
    static constexpr auto FlagNoMouseInputs = ImGuiWindowFlags_NoMouseInputs;
    static constexpr auto FlagMenuBar = ImGuiWindowFlags_MenuBar;
    static constexpr auto FlagHorizontalScrollbar = ImGuiWindowFlags_HorizontalScrollbar;
    static constexpr auto FlagNoFocusOnAppearing = ImGuiWindowFlags_NoFocusOnAppearing;
    static constexpr auto FlagNoBringToFrontOnFocus = ImGuiWindowFlags_NoBringToFrontOnFocus;
    static constexpr auto FlagAlwaysVerticalScrollbar = ImGuiWindowFlags_AlwaysVerticalScrollbar;
    static constexpr auto FlagAlwaysHorizontalScrollbar = ImGuiWindowFlags_AlwaysHorizontalScrollbar;
    static constexpr auto FlagNoNavInputs = ImGuiWindowFlags_NoNavInputs;
    static constexpr auto FlagNoNavFocus = ImGuiWindowFlags_NoNavFocus;
    static constexpr auto FlagUnsavedDocument = ImGuiWindowFlags_UnsavedDocument;
    static constexpr auto FlagNoNav = ImGuiWindowFlags_NoNav;
    static constexpr auto FlagNoDecoration = ImGuiWindowFlags_NoDecoration;

public:
    Window(std::string title, float x, float y, float w, float h, int flags = 0);
    ~Window() override = default;

public:
    const std::string& title() const {
        return title_;
    }

    void setTitle(const std::string& v) {
        if (title_ != v) {
            title_ = v;
        }
    }

    int flags() const {
        return flags_;
    }


    const Eigen::Vector2f& position() const {
        return position_;
    }

    void move(float x, float y) {
        move({ x, y });
    }

    void move(const Eigen::Vector2f& v) {
        if (!position_.isApprox(v)) {
            position_ = v;
            state_ |= StatePositionDirty;
        }
    }

    const Eigen::Vector2f& size() const {
        return size_;
    }

    void resize(float w, float h) {
        resize({ w, h });
    }

    void resize(const Eigen::Vector2f& v) {
        if (!size_.isApprox(v)) {
            size_ = v;
            state_ |= StateSizeDirty;
        }
    }

    MenuBar& menuBar() {
        VOLCANO_ASSERT(flags_ & ImGuiWindowFlags_MenuBar);
        return menu_bar_;
    }

    const MenuBar& menuBar() const {
        VOLCANO_ASSERT(flags_ & ImGuiWindowFlags_MenuBar);
        return menu_bar_;
    }

protected:
    void paint() override;

private:
    enum {
        StatePositionDirty = 0x1,
        StateSizeDirty = 0x2
    };

    int state() const {
        return state_;
    }

private:
    std::string title_;
    int flags_;
    int state_;
    Eigen::Vector2f position_;
    Eigen::Vector2f size_;
    MenuBar menu_bar_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WINDOW_H
