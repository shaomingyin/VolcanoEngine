#include <utility>

#include <Volcano/Framework/GuiContext.h>

VOLCANO_FRAMEWORK_BEGIN

GuiContext::GuiContext(Graphics::Canvas& canvas, Fonts fonts, Images images)
    : canvas_(canvas), fonts_(std::move(fonts)), images_(std::move(images)) {
    VOLCANO_ASSERT(fonts_ && images_);
}

void GuiContext::save() {
    canvas_.save();
}

void GuiContext::restore() {
    canvas_.restore();
}

void GuiContext::setClip(const Rectf& rect) {
    canvas_.setClip(rect);
}

void GuiContext::setColor(const Colorf& color) {
    canvas_.setColor(color);
}

void GuiContext::beginPath() {
    canvas_.beginPath();
}

void GuiContext::moveTo(float x, float y) {
    canvas_.moveTo(x,y);
}

void GuiContext::lineTo(float x, float y) {
    canvas_.lineTo(x,y);
}

void GuiContext::bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
    canvas_.bezierTo(c1x,c1y,c2x,c2y,x,y);
}

void GuiContext::closePath() {
    canvas_.closePath();
}

void GuiContext::rect(const Rectf& rect) {
    canvas_.rect(rect.left,rect.top,rect.width,rect.height);
}

void GuiContext::fill() {
    canvas_.fill();
}

void GuiContext::stroke(float width) {
    canvas_.setStrokeWidth(width);
    canvas_.stroke();
}

void GuiContext::image(std::string_view resource, const Rectf& destination) {
    canvas_.image(images_(resource),destination);
}

void GuiContext::text(const std::filesystem::path& font, float pixels, float x, float baseline, std::string_view text) {
    canvas_.setFont(fonts_(font),pixels);
    canvas_.text(x,baseline,text);
}

Sizef GuiContext::measure(const std::filesystem::path& font, float pixels, std::string_view text) {
    return fonts_(font).measure(text,pixels);
}

VOLCANO_FRAMEWORK_END
