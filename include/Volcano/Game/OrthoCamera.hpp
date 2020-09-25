//
//
#ifndef VOLCANO_GAME_ORTHOCAMERA_HPP
#define VOLCANO_GAME_ORTHOCAMERA_HPP

#include <Volcano/Game/Camera.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_API OrthoCamera: public Camera
{
    Q_OBJECT

public:
    OrthoCamera(QObject *parent = nullptr);
    ~OrthoCamera(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ORTHOCAMERA_HPP
