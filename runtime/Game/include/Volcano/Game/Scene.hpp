//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <memory>
#include <vector>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Transform.hpp>
#include <Volcano/Game/Dynamic.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Scene: public Object {
public:
	using Entities = std::vector<std::unique_ptr<Entity>>;

public:
	Scene();
	virtual ~Scene();

public:
	Entities& entities() noexcept {
		return entities_;
	}

	const Entities& entities() const noexcept {
		return entities_;
	}

	void update(Clock::duration elapsed, Scheduler& scheduler) override;
	Graphics::VisibleSet buildVisibleSet(Scheduler& scheduler) const override;

private:
	Transform transform_;
	Entities entities_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_HPP
