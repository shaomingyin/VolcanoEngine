//
//
#ifndef VOLCANO_SCENE_HPP
#define VOLCANO_SCENE_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>
#include <Volcano/View.hpp>
#include <Volcano/Camera.hpp>

VOLCANO_BEGIN

class VOLCANO_API Scene: public Node {
public:
	Scene(Node *parent = nullptr);
	~Scene(void) override;

public:
    void buildView(View &view, Camera &camera);
};

VOLCANO_END

#endif // VOLCANO_SCENE_HPP
