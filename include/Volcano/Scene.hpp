//
//
#ifndef VOLCANO_SCENE_HPP
#define VOLCANO_SCENE_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>
#include <Volcano/Camera.hpp>

VOLCANO_BEGIN

class VOLCANO_API Scene: public Node
{
    Q_OBJECT

public:
	Scene(Node *parent = nullptr);
	~Scene(void) override;

protected:
    void onTick(float elapsed) override;
};

VOLCANO_END

#endif // VOLCANO_SCENE_HPP
