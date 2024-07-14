//
//
#ifndef VOLCANO_GRAPHICS_VIEW_HPP
#define VOLCANO_GRAPHICS_VIEW_HPP

#include <QRectF>
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

    void lookTo(const Eigen::Vector3f& eye, const Eigen::Vector3f& dir, const Eigen::Vector3f& up) {
        lookAt(eye, eye + dir, up);
    }

    void lookAt(const Eigen::Vector3f& eye, const Eigen::Vector3f& target, const Eigen::Vector3f& up) {
        eye_ = eye;
        target_ = target.normalized();
        up_ = up.normalized();
    }

    Eigen::Matrix4x4f viewMatrix() const {
        return Eigen::lookAt(eye_, target_, up_);
    }

    void perspective(float fov_y, float aspect_ratio, float near_plane, float far_plane) {
        fov_y_ = fov_y;
        aspect_ratio_ = aspect_ratio;
        near_plane_ = near_plane;
        far_plane_ = far_plane;
    }

    auto projectMatrix() const {
        return Eigen::perspective(fov_y_, aspect_ratio_, near_plane_, far_plane_);
    }

    const QRectF& viewport() const {
        return viewport_;
    }

    void setViewport(const QRectF& v) {
        viewport_ = v;
    }

private:
    bool framing_;
    Eigen::Vector3f eye_;
    Eigen::Vector3f target_;
    Eigen::Vector3f up_;
    float fov_y_;
    float aspect_ratio_;
    float near_plane_;
    float far_plane_;
    QRectF viewport_;
    QVector<FrameData> frame_data_vector_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEW_HPP
