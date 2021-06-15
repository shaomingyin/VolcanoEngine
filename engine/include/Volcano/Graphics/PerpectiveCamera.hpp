//
//
#ifndef VOLCANO_GRAPHICS_PERPECTIVECAMERA_HPP
#define VOLCANO_GRAPHICS_PERPECTIVECAMERA_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>

VOLCANO_GRAPHICS_BEGIN

class PerpectiveCamera: public Camera {
    Q_OBJECT
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged)

public:
    PerpectiveCamera(QObject *parent = nullptr);
    ~PerpectiveCamera(void) override;

public:
    float fov(void) const;
    void setFov(float v);
    void calcProjectMatrix(QMatrix4x4 &out) override;

signals:
    void fovChanged(float v);

private:
    float m_fov;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PERPECTIVECAMERA_HPP
