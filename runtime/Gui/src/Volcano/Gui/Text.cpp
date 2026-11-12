#include <algorithm>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <utility>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Gui/Text.h>

VOLCANO_GUI_BEGIN

void Text::setText(std::string value) {
    text_ = std::move(value);
    changed();
}

void Text::setFont(std::filesystem::path value) {
    font_ = std::move(value);
    changed();
}

void Text::setFontSize(float value) {
    if (!(value >= 1 && value <= 4096)) {
        throw std::invalid_argument("Invalid GUI font size");
    }
    pixels_ = value;
    changed();
}

void Text::setColor(const Colorf& value) {
    color_ = value;
    changed();
}

void Text::setWrap(bool value) {
    wrap_ = value;
    changed();
}

std::vector<std::string> Text::lines(Platform& platform, float width) const {
    const float pixels = fontSize(platform);
    std::vector<std::string> result;
    std::string line;
    for (std::size_t i = 0; i < text_.size();) {
        if (text_[i] == '\n') {
            result.push_back(std::move(line));
            line.clear();
            ++i;
            continue;
        }
        const auto start = i++;
        while (i < text_.size() && (static_cast<unsigned char>(text_[i]) & 0xc0) == 0x80) {
            ++i;
        }
        const auto character = text_.substr(start, i - start);
        if (wrap_ && !line.empty() && platform.measure(font_, pixels, line + character).width > width) {
            result.push_back(std::move(line));
            line.clear();
        }
        line += character;
    }
    result.push_back(std::move(line));
    return result;
}

Sizef Text::measure(Platform& platform, Sizef available) const {
    auto result = Widget::measure(platform, available);
    if (widthSizing() == Sizing::Fixed && heightSizing() == Sizing::Fixed) {
        return result;
    }
    const float width = widthSizing() == Sizing::Fixed ? frame().width : available.width;
    const float pixels = fontSize(platform);
    float naturalWidth = 0;
    float naturalHeight = 0;
    if (!text_.empty()) {
        const auto content = lines(platform, width);
        for (const auto& line : content) {
            const auto size = platform.measure(font_, pixels, line);
            if (!std::isfinite(size.width) || !std::isfinite(size.height) || size.width < 0 ||
                size.height < 0) {
                throw std::runtime_error("GUI platform returned an invalid text size");
            }
            naturalWidth = std::max(naturalWidth, size.width);
        }
        naturalHeight = static_cast<float>(content.size()) * pixels * platform.style().lineHeight();
    }
    if (widthSizing() != Sizing::Fixed) {
        result.width = widthSizing() == Sizing::Fill ? available.width : naturalWidth;
    }
    if (heightSizing() != Sizing::Fixed) {
        result.height = heightSizing() == Sizing::Fill ? available.height : naturalHeight;
    }
    if (!std::isfinite(result.width) || !std::isfinite(result.height)) {
        throw std::overflow_error("GUI text size overflow");
    }
    return result;
}

void Text::paint(World::Canvas& canvas, Platform& platform) const {
    Widget::paint(canvas, platform);
    if (text_.empty()) {
        return;
    }
    canvas.save();
    auto restore = scopeGuard([&canvas] {
        canvas.restore();
    });
    canvas.setClip(bounds());
    const auto& style = platform.style();
    canvas.setColor(
        color_.value_or(root().acceptsInput(*this) ? style.textColor() : style.disabledTextColor()));
    const float pixels = fontSize(platform);
    float baseline = bounds().top + pixels;
    for (const auto& line : lines(platform, bounds().width)) {
        if (!std::isfinite(baseline)) {
            throw std::overflow_error("GUI text position overflow");
        }
        canvas.text(font_.string(), pixels, bounds().left, baseline, line);
        baseline += pixels * style.lineHeight();
    }
}

void Text::collectResources(std::vector<std::filesystem::path>& fonts,
                            std::vector<std::string>& images) const {
    Widget::collectResources(fonts, images);
    if (!font_.empty()) {
        fonts.push_back(font_);
    }
}

VOLCANO_GUI_END
