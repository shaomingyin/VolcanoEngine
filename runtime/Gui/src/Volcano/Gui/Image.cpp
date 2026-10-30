#include <utility>

#include <Volcano/Gui/Image.h>

VOLCANO_GUI_BEGIN

void Image::setSource(std::string value) {
    source_ = std::move(value);
    changed();
}

void Image::paint(Platform& platform) const {
    Widget::paint(platform);
    if (!source_.empty()) {
        platform.setColor(platform.style().imageColor());
        platform.image(source_, bounds());
    }
}

void Image::collectResources(std::vector<std::filesystem::path>& fonts,
                             std::vector<std::string>& images) const {
    Widget::collectResources(fonts, images);
    if (!source_.empty()) {
        images.push_back(source_);
    }
}

VOLCANO_GUI_END
