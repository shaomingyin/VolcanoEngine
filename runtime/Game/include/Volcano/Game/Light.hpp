//
//
#ifndef VOLCANO_GAME_LIGHT_HPP
#define VOLCANO_GAME_LIGHT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

class Light: public Component {
public:
	Light();

public:
	const Eigen::Vector4f& color() const noexcept {
		return color_;
	}

private:
	Eigen::Vector4f color_;
	float strength_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_HPP
