#ifndef VOLCANO_GUI_IMAGE_H
#define VOLCANO_GUI_IMAGE_H

#include <string>

#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

class Image final : public Widget {
public:
    using Widget::Widget;
    const std::string& source() const noexcept {
        return source_;
    }
    void setSource(std::string value);
    void collectResources(std::vector<std::filesystem::path>& fonts,
                          std::vector<std::string>& images) const override;

protected:
    void paint(Platform& platform) const override;

private:
    std::string source_;
};

VOLCANO_GUI_END

#endif
