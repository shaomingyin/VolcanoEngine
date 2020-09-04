//
//
#ifndef VOLCANO_SCENE_PERPECTIVECAMERA_HPP
#define VOLCANO_SCENE_PERPECTIVECAMERA_HPP

#include <Volcano/Scene/Camera.hpp>

VOLCANO_SCENE_BEGIN

class VOLCANO_API PerpectiveCamera: public Camera
{
    Q_OBJECT
    Q_PROPERTY(qreal fov READ fov WRITE setFov NOTIFY fovChanged)

public:
    PerpectiveCamera(QObject *parent = nullptr);
    ~PerpectiveCamera(void) override;

public:
    qreal fov(void) const;
    void setFov(qreal v);

signals:
    void fovChanged(qreal v);

private:
    qreal m_fov;
};

VOLCANO_INLINE qreal PerpectiveCamera::fov(void) const
{
    return m_fov;
}

VOLCANO_INLINE void PerpectiveCamera::setFov(qreal v)
{
    if (!qFuzzyCompare(m_fov, v))
    {
        m_fov = v;
        fovChanged(v);
    }
}

VOLCANO_SCENE_END

#endif // VOLCANO_SCENE_PERPECTIVECAMERA_HPP
