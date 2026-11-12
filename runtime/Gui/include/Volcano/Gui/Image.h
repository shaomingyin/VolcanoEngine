#ifndef VOLCANO_GUI_IMAGE_H
#define VOLCANO_GUI_IMAGE_H

#include <memory>
#include <string>

#include <Volcano/World/Texture.h>
#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

class Image final : public Widget {
public:
    using Widget::Widget;
    const std::string& source() const noexcept {
        return source_;
    }
    void setSource(std::string value);
    std::shared_ptr<World::Texture> texture() const noexcept {
        return texture_;
    }
    void setTexture(std::shared_ptr<World::Texture> texture);
    void collectResources(std::vector<std::filesystem::path>& fonts,
                          std::vector<std::string>& images) const override;

protected:
    void paint(World::Canvas& canvas, Platform& platform) const override;

private:
    std::string source_;
    std::shared_ptr<World::Texture> texture_;
};

VOLCANO_GUI_END

#endif
