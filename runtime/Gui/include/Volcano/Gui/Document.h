#ifndef VOLCANO_GUI_DOCUMENT_H
#define VOLCANO_GUI_DOCUMENT_H

#include <memory>

#include <Volcano/Size.h>
#include <Volcano/World/Canvas.h>
#include <Volcano/Gui/Layout.h>
#include <Volcano/Gui/Event.h>
#include <Volcano/Gui/Platform.h>

VOLCANO_GUI_BEGIN

class Document final : public World::Canvas, private Widget::Observer {
public:
    // The caller keeps platform alive longer than this document; documents may share it.
    Document(Platform& platform, std::unique_ptr<Layout> root, Size size = Size{});
    explicit Document(std::unique_ptr<Layout> root, Size size = Size{});
    ~Document() override;
    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;
    Document(Document&&) = delete;
    Document& operator=(Document&&) = delete;
    Platform& platform() const noexcept {
        return platform_;
    }
    Layout& root() noexcept {
        return *root_;
    }
    const Layout& root() const noexcept {
        return *root_;
    }
    std::uint64_t revision() const noexcept {
        return root_->revision();
    }
    void setSize(Size size);
    void layout(Sizef size);
    void draw();
    bool event(const PointerEvent& event);
    bool event(const ScrollEvent& event);
    bool event(const KeyEvent& event);
    bool event(const TextInputEvent& event);
    bool event(const CompositionEvent& event);
    bool event(const FocusEvent& event);
    const Widget* focus() const noexcept {
        return focus_;
    }

private:
    void remove(Widget& subtree) override;
    void destroy(Widget& subtree) noexcept override;
    bool busy() const noexcept override {
        return dispatching_;
    }
    void setFocus(Widget* widget);
    bool setHover(Widget* widget, const PointerEvent& input);
    bool cancelCapture();
    Widget* focusedWidget();

private:
    Platform& platform_;
    std::unique_ptr<Layout> root_;
    Widget* captured_ = nullptr;
    Widget* focus_ = nullptr;
    Widget* hovered_ = nullptr;
    PointerEvent captured_event_;
    PointerEvent hovered_event_;
    bool active_ = true;
    mutable bool dispatching_ = false;
};

VOLCANO_GUI_END

#endif
