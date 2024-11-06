//
//
#ifndef VOLCANO_WORLD_COMPONENT_H
#define VOLCANO_WORLD_COMPONENT_H

#include <Volcano/World/Common.h>
#include <Volcano/World/RigidBody.h>

VOLCANO_WORLD_BEGIN

template <typename T>
concept RigidBodyType = std::is_base_of_v<RigidBody, T>;

template <typename T>
concept PositionalType = requires(const T & c) {
	{ c.offset() } -> std::same_as<const Eigen::Affine3f&>;
};

template <typename T>
concept PositionalRigidBodyType = (PositionalType<T> && RigidBodyType<T>);

template <typename T>
concept MovableType = (PositionalType<T> &&
	requires(T& c) { { c.offset() } -> std::same_as<Eigen::Affine3f&>; }
);

template <typename T>
concept MovableRigidBodyType = (MovableType<T> && RigidBodyType<T>);

class Positional {
public:
	Positional()
		: offset_(Eigen::Affine3f::Identity()) {
	}

	virtual ~Positional() = default;

public:
	const Eigen::Affine3f& offset() const {
		return offset_;
	}

protected:
	Eigen::Affine3f offset_;
};

class Movable: public Positional {
public:
	Movable() = default;
	virtual ~Movable() = default;

public:
	Eigen::Affine3f& offset() {
		return offset_;
	}
};


VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COMPONENT_H
