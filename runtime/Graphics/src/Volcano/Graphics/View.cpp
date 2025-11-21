//
//
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

View::View() {
    set(Bool::Clear, true);
    set(Color::Clear, sf::Color::Black);
    set(Color::AmbientLight, sf::Color::White);
    set(Affine3f::View, Eigen::Affine3f::Identity());
}

VOLCANO_GRAPHICS_END
