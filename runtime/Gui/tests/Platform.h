#ifndef VOLCANO_GUI_TEST_PLATFORM_H
#define VOLCANO_GUI_TEST_PLATFORM_H

#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#include <Volcano/Color.h>
#include <Volcano/Rect.h>
#include <Volcano/World/Canvas.h>
#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Platform.h>
#include <Volcano/Gui/Widget.h>
#include <Volcano/Gui/Document.h>

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

    Volcano::Sizef measure(const std::filesystem::path&, float pixels, std::string_view text) override {
        const auto count = std::count_if(text.begin(), text.end(), [](unsigned char value) {
            return (value & 0xc0) != 0x80;
        });
        return {static_cast<float>(count) * pixels * 0.5f, pixels * 1.2f};
    }

    void inspect(const Volcano::World::Canvas& canvas) {
        for (const auto& cmd : canvas.commands()) {
            colors.push_back(cmd.state.inner);
            if (cmd.type == Volcano::World::Canvas::Command::Type::Fill) {
                ++fills;
                if (!cmd.path.empty()) {
                    float minX = std::numeric_limits<float>::max();
                    float minY = std::numeric_limits<float>::max();
                    float maxX = std::numeric_limits<float>::lowest();
                    float maxY = std::numeric_limits<float>::lowest();
                    for (const auto& seg : cmd.path) {
                        if (seg.verb != Volcano::World::Canvas::Verb::Close) {
                            minX = std::min(minX, seg.point.x());
                            minY = std::min(minY, seg.point.y());
                            maxX = std::max(maxX, seg.point.x());
                            maxY = std::max(maxY, seg.point.y());
                        }
                    }
                    rectangles.push_back(Volcano::Rectf{minX, minY, maxX - minX, maxY - minY});
                }
            } else if (cmd.type == Volcano::World::Canvas::Command::Type::Stroke) {
                strokes.push_back(cmd.state.strokeWidth);
                if (!cmd.path.empty()) {
                    float minX = std::numeric_limits<float>::max();
                    float minY = std::numeric_limits<float>::max();
                    float maxX = std::numeric_limits<float>::lowest();
                    float maxY = std::numeric_limits<float>::lowest();
                    for (const auto& seg : cmd.path) {
                        if (seg.verb != Volcano::World::Canvas::Verb::Close) {
                            minX = std::min(minX, seg.point.x());
                            minY = std::min(minY, seg.point.y());
                            maxX = std::max(maxX, seg.point.x());
                            maxY = std::max(maxY, seg.point.y());
                        }
                    }
                    rectangles.push_back(Volcano::Rectf{minX, minY, maxX - minX, maxY - minY});
                }
            } else if (cmd.type == Volcano::World::Canvas::Command::Type::Text) {
                drawnText.push_back(cmd.text);
                textSizes.push_back(cmd.pixels);
                textBaselines.push_back(cmd.baseline);
            } else if (cmd.type == Volcano::World::Canvas::Command::Type::Image) {
                rectangles.push_back(cmd.destination);
            }
        }
    }

    void draw(Volcano::Gui::Document& doc) {
        doc.draw();
        inspect(doc);
    }

    void draw(const Volcano::Gui::Widget& widget) {
        Volcano::World::Canvas canvas;
        canvas.begin();
        widget.draw(canvas, *this);
        canvas.end();
        inspect(canvas);
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