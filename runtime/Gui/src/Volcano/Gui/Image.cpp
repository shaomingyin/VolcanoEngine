#include <utility>

#include <Volcano/Gui/Image.h>

VOLCANO_GUI_BEGIN

void Image::setSource(std::string value) {
    source_ = std::move(value);
    changed();
}

void Image::setTexture(std::shared_ptr<World::Texture> texture) {
    texture_ = std::move(texture);
    changed();
}

void Image::paint(World::Canvas& canvas, Platform& platform) const {
    Widget::paint(canvas, platform);
    if (bounds().width > 0 && bounds().height > 0) {
        if (texture_) {
            canvas.setColor(platform.style().imageColor());
            canvas.image(texture_, bounds());
        } else if (!source_.empty()) {
            canvas.setColor(platform.style().imageColor());
            canvas.image(source_, bounds());
        }
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
