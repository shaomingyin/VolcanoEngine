//
//
#ifndef VOLCANO_ORTHOCAMERA_HPP
#define VOLCANO_ORTHOCAMERA_HPP

#include <Volcano/Camera.hpp>

VOLCANO_BEGIN

class VOLCANO_API OrthoCamera: public Camera
{
    Q_OBJECT

public:
    OrthoCamera(QObject *parent = nullptr);
    ~OrthoCamera(void) override;
};

VOLCANO_END

#endif // VOLCANO_ORTHOCAMERA_HPP
