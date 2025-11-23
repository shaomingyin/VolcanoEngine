//
//
#ifndef VOLCANO_FUSION_DISPLAY_HPP
#define VOLCANO_FUSION_DISPLAY_HPP

#include <Volcano/Graphics/Canvas.hpp>
#include <Volcano/Gui/Object.hpp>
#include <Volcano/Fusion/Common.hpp>

VOLCANO_FUSION_BEGIN

class Display: public Graphics::Canvas {
public:
    Display(Graphics::Context& context);
    ~Display() override = default;

protected:
    void setGuiObject(Gui::Object& obj) {
        gui_object_ = &obj;
    }

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) override;

private:
    Gui::Object* gui_object_;
};

VOLCANO_FUSION_END

#endif // VOLCANO_FUSION_DISPLAY_HPP
