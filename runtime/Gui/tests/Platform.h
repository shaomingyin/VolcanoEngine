#ifndef VOLCANO_GUI_TEST_PLATFORM_H
#define VOLCANO_GUI_TEST_PLATFORM_H

#include <algorithm>
#include <string>
#include <vector>

#include <Volcano/Gui/Platform.h>

namespace {
class TestPlatform final : public Volcano::Gui::Platform {
public:
    const Volcano::Gui::Style& style() const noexcept override {
        return currentStyle;
    }
    void setCursor(Cursor) override {}
    std::string clipboardText() override {
        return {};
    }
    void setClipboardText(std::string_view) override {}
    void startTextInput() override {}
    void stopTextInput() override {}
    void setTextInputRect(const Volcano::Rectf&) override {}
    void setPointerCapture(bool) override {}
    void save() override {}
    void restore() override {}
    void setClip(const Volcano::Rectf&) override {}
    void setColor(const Volcano::Colorf& color) override {
        colors.push_back(color);
    }
    void beginPath() override {}
    void moveTo(float, float) override {}
    void lineTo(float, float) override {}
    void bezierTo(float, float, float, float, float, float) override {}
    void closePath() override {}
    void rect(const Volcano::Rectf& rect) override {
        rectangles.push_back(rect);
    }
    void fill() override {
        ++fills;
    }
    void stroke(float width) override {
        strokes.push_back(width);
    }
    void image(std::string_view, const Volcano::Rectf&) override {}
    void text(const std::filesystem::path&, float pixels, float, float baseline,
              std::string_view value) override {
        drawnText.emplace_back(value);
        textSizes.push_back(pixels);
        textBaselines.push_back(baseline);
    }
    Volcano::Sizef measure(const std::filesystem::path&, float pixels, std::string_view text) override {
        const auto count = std::count_if(text.begin(), text.end(), [](unsigned char value) {
            return (value & 0xc0) != 0x80;
        });
        return {static_cast<float>(count) * pixels * 0.5f, pixels * 1.2f};
    }

public:
    Volcano::Gui::Style currentStyle;
    std::vector<Volcano::Colorf> colors;
    std::vector<Volcano::Rectf> rectangles;
    std::vector<float> strokes;
    std::vector<float> textSizes;
    std::vector<float> textBaselines;
    int fills = 0;
    std::vector<std::string> drawnText;
};

} // namespace

#endif
