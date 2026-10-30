#ifndef VOLCANO_GUI_WIDGET_H
#define VOLCANO_GUI_WIDGET_H

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <Volcano/Color.h>
#include <Volcano/Rect.h>
#include <Volcano/Gui/Event.h>
#include <Volcano/Gui/Platform.h>

VOLCANO_GUI_BEGIN

class Widget {
public:
    enum class Sizing { Fixed, Auto, Fill };
    enum class Alignment { Auto, Start, Center, End };
    struct Insets {
        float left = 0;
        float top = 0;
        float right = 0;
        float bottom = 0;
    };

    class Observer {
    public:
        virtual ~Observer() = default;
        virtual void remove(Widget& subtree) = 0;
        virtual void destroy(Widget& subtree) noexcept = 0;
        virtual bool busy() const noexcept = 0;
    };

    explicit Widget(Widget* parent = nullptr);
    virtual ~Widget();
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
    Widget(Widget&&) = delete;
    Widget& operator=(Widget&&) = delete;

    // A parent owns its children; detaching transfers lifetime management to the caller.
    void setParent(Widget* parent);
    const std::vector<Widget*>& children() const noexcept {
        return children_;
    }
    Widget* parent() noexcept {
        return parent_;
    }
    const Widget* parent() const noexcept {
        return parent_;
    }
    Widget& root() noexcept;
    const Widget& root() const noexcept;
    bool contains(const Widget& widget) const noexcept;
    // One observer per root; it must unregister before its lifetime ends.
    void observe(Observer& observer);
    void unobserve(Observer& observer) noexcept;
    const Rectf& frame() const noexcept {
        return frame_;
    }
    const Rectf& bounds() const noexcept {
        return bounds_;
    }
    void setFrame(const Rectf& value);
    void setWidth(float value);
    void setHeight(float value);
    void setWidth(Sizing value);
    void setHeight(Sizing value);
    Sizing widthSizing() const noexcept {
        return width_sizing_;
    }
    Sizing heightSizing() const noexcept {
        return height_sizing_;
    }
    void setMargin(float value);
    void setMargin(const Insets& value);
    const Insets& margin() const noexcept {
        return margin_;
    }
    void setHorizontalAlignment(Alignment value);
    void setVerticalAlignment(Alignment value);
    Alignment horizontalAlignment() const noexcept {
        return horizontal_alignment_;
    }
    Alignment verticalAlignment() const noexcept {
        return vertical_alignment_;
    }
    void setBreakBefore(bool value);
    bool breakBefore() const noexcept {
        return break_before_;
    }
    bool visible() const noexcept {
        return visible_;
    }
    void setVisible(bool value);
    bool enabled() const noexcept {
        return enabled_;
    }
    void setEnabled(bool value);
    const Colorf& background(const Platform& platform) const noexcept {
        return background_ ? *background_ : platform.style().backgroundColor();
    }
    void setBackground(const Colorf& value);
    std::uint64_t revision() const noexcept {
        return revision_;
    }
    // available is a finite, nonnegative size; measure does not change the arranged bounds.
    virtual Sizef measure(Platform& platform, Sizef available) const;
    virtual void layout(const Rectf& bounds, Platform& platform);
    void draw(Platform& platform) const;
    virtual Widget* hit(float x, float y);
    bool acceptsInput(const Widget& target) const noexcept;
    virtual bool focusable() const noexcept {
        return false;
    }
    virtual bool event(const PointerEvent& event, Platform& platform);
    virtual bool event(const ScrollEvent& event, Platform& platform);
    virtual bool event(const KeyEvent& event, Platform& platform);
    virtual bool event(const TextInputEvent& event, Platform& platform);
    virtual bool event(const CompositionEvent& event, Platform& platform);
    virtual bool event(const FocusEvent& event, Platform& platform);
    virtual void collectResources(std::vector<std::filesystem::path>& fonts,
                                  std::vector<std::string>& images) const;

protected:
    void changed() noexcept;
    void setBounds(const Rectf& bounds);
    virtual void paint(Platform& platform) const;

private:
    void checkTreeChange() const;

private:
    std::vector<Widget*> children_;
    Widget* parent_ = nullptr;
    Observer* observer_ = nullptr;
    bool changing_tree_ = false;
    Rectf frame_{};
    Rectf bounds_{};
    Sizing width_sizing_ = Sizing::Auto;
    Sizing height_sizing_ = Sizing::Auto;
    Insets margin_;
    Alignment horizontal_alignment_ = Alignment::Auto;
    Alignment vertical_alignment_ = Alignment::Auto;
    bool break_before_ = false;
    std::optional<Colorf> background_;
    bool visible_ = true;
    bool enabled_ = true;
    std::uint64_t revision_ = 1;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WIDGET_H
