//
//
#ifndef VOLCANO_GRAPHICS_VIEW_HPP
#define VOLCANO_GRAPHICS_VIEW_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/FrameData.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_GRAPHICS_API View: public QObject {
public:
    View(QObject* parent = nullptr);

public:
    virtual bool beginFrame() {
        Q_ASSERT(!framing_);
        frame_data_vector_.clear();
        framing_ = true;
        return true;
    }

    virtual void commitFrame() {
        Q_ASSERT(!framing_);
        framing_ = false;
    }

    virtual void cancelFrame() {
        Q_ASSERT(!framing_);
        framing_ = false;
    }

    FrameData& allocateFrameData() {
        return frame_data_vector_.emplaceBack();
    }

    void lookTo(const QVector3D& eye, const QVector3D& dir, const QVector3D& up) {
        lookAt(eye, eye + dir, up);
    }

    void lookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up) {
        eye_ = eye;
        center_ = center.normalized();
        up_ = up.normalized();
    }

    void buildViewMatrix(QMatrix4x4& v) const {
        v.lookAt(eye_, center_, up_);
    }

    void perspective(float vertical_fov, float aspect_rate, float near_plane, float far_plane) {
        vertical_fov_ = vertical_fov;
        aspect_rate_ = aspect_rate;
        near_plane_ = near_plane;
        far_plane_ = far_plane;
    }

    void buildProjectMatrix(QMatrix4x4& v) const {
        v.perspective(vertical_fov_, aspect_rate_, near_plane_, far_plane_);
    }

    const QVector4D& viewport() const {
        return viewport_;
    }

    void setViewport(const QVector4D& v) {
        viewport_ = v;
    }

private:
    bool framing_;
    QVector3D eye_;
    QVector3D center_;
    QVector3D up_;
    float vertical_fov_;
    float aspect_rate_;
    float near_plane_;
    float far_plane_;
    QVector4D viewport_;
    QVector<FrameData> frame_data_vector_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
