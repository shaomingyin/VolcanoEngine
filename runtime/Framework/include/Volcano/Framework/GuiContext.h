#ifndef VOLCANO_FRAMEWORK_GUICONTEXT_H
#define VOLCANO_FRAMEWORK_GUICONTEXT_H

#include <functional>

#include <Volcano/Graphics/Canvas.h>
#include <Volcano/Gui/Context.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

class GuiContext final : public Gui::Context {
public:
    using Fonts = std::function<const Graphics::Font&(const std::filesystem::path&)>;
    using Images = std::function<const Graphics::Texture&(std::string_view)>;
    GuiContext(Graphics::Canvas& canvas, Fonts fonts, Images images);
    void save() override;
    void restore() override;
    void setClip(const Rectf& rect) override;
    void setColor(const Colorf& color) override;
    void beginPath() override;
    void moveTo(float x, float y) override;
    void lineTo(float x, float y) override;
    void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) override;
    void closePath() override;
    void rect(const Rectf& rect) override;
    void fill() override;
    void stroke(float width) override;
    void image(std::string_view resource, const Rectf& destination) override;
    void text(const std::filesystem::path& font, float pixels, float x, float baseline, std::string_view text) override;
    Sizef measure(const std::filesystem::path& font, float pixels, std::string_view text) override;

private:
    Graphics::Canvas& canvas_;
    Fonts fonts_;
    Images images_;
};

VOLCANO_FRAMEWORK_END

#endif
