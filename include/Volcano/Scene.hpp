//
//
#ifndef VOLCANO_SCENE_HPP
#define VOLCANO_SCENE_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

class Scene: public Node {
public:
	Scene(Node *parent = nullptr);
	~Scene(void) override;
};

VOLCANO_END

#endif // VOLCANO_SCENE_HPP
