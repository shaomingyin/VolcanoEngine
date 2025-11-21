//
//
#ifndef VOLCANO_UTILS_DISPLAY_HPP
#define VOLCANO_UTILS_DISPLAY_HPP

#include <Volcano/Graphics/Canvas.hpp>
#include <Volcano/Gui/Object.hpp>
#include <Volcano/Utils/Common.hpp>

VOLCANO_UTILS_BEGIN

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

VOLCANO_UTILS_END

#endif // VOLCANO_UTILS_DISPLAY_HPP
