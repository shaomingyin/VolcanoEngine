//
//
#ifndef VOLCANO_GAME_OBJECT_H
#define VOLCANO_GAME_OBJECT_H

#include <string>

#include <Volcano/Game/Common.h>
#include <Volcano/Game/Context.h>

VOLCANO_GAME_BEGIN

class Object {
public:
	Object(Context& context);
	virtual ~Object();

public:
	Context& context() {
		return context_;
	}

	const Context& context() const {
		return context_;
	}

	entt::registry& registry() {
		return context_.registry();
	}

	const entt::registry& registry() const {
		return context_.registry();
	}

	entt::entity entity() const {
		return entity_;
	}

	const std::string& name() const {
		return registry().get<Data>(entity_).name;
	}

	void setName(const std::string& v) {
		registry().patch<Data>(entity_, [&v](auto& d) { d.name = v; });
	}

private:
	struct Data {
		std::string name;
	};

private:
	Context& context_;
	entt::entity entity_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_H
