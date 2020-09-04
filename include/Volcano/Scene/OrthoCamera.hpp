//
//
#ifndef VOLCANO_SCENE_ORTHOCAMERA_HPP
#define VOLCANO_SCENE_ORTHOCAMERA_HPP

#include <Volcano/Scene/Camera.hpp>

VOLCANO_SCENE_BEGIN

class VOLCANO_API OrthoCamera: public Camera
{
    Q_OBJECT

public:
    OrthoCamera(QObject *parent = nullptr);
    ~OrthoCamera(void) override;
};

VOLCANO_SCENE_END

#endif // VOLCANO_SCENE_ORTHOCAMERA_HPP
