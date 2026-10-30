#ifndef VOLCANO_GUI_TEXT_H
#define VOLCANO_GUI_TEXT_H

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

class Text : public Widget {
public:
    using Widget::Widget;
    const std::string& text() const noexcept {
        return text_;
    }
    void setText(std::string value);
    const std::filesystem::path& font() const noexcept {
        return font_;
    }
    void setFont(std::filesystem::path value);
    float fontSize(const Platform& platform) const noexcept {
        return pixels_.value_or(platform.style().fontSize());
    }
    void setFontSize(float value);
    void setColor(const Colorf& value);
    void setWrap(bool value);
    Sizef measure(Platform& platform, Sizef available) const override;
    void collectResources(std::vector<std::filesystem::path>& fonts,
                          std::vector<std::string>& images) const override;

protected:
    void paint(Platform& platform) const override;

private:
    std::vector<std::string> lines(Platform& platform, float width) const;

private:
    std::string text_;
    std::filesystem::path font_;
    std::optional<float> pixels_;
    std::optional<Colorf> color_;
    bool wrap_ = false;
};

VOLCANO_GUI_END

#endif
