//
//
#include <Volcano/World/Camera.h>

VOLCANO_WORLD_BEGIN

Camera::Camera(QObject* parent)
    : Transformable(parent) {
    view_matrix_.setToIdentity();
    projection_matrix_.setToIdentity();
}

void Camera::reset() noexcept {
    view_matrix_.setToIdentity();
    projection_matrix_.setToIdentity();
}

const QMatrix4x4& Camera::viewMatrix() const noexcept {
    return view_matrix_;
}

const QMatrix4x4& Camera::projectionMatrix() const noexcept {
    return projection_matrix_;
}

const QRectF& Camera::rect() const noexcept {
    return rect_;
}

void Camera::setRect(const QRectF& v) noexcept {
    if (!qFuzzyCompare(rect_, v)) {
        rect_ = v.normalized();
        emit rectChanged(rect_);
    }
}

float Camera::fov() const noexcept {
    return fov_;
}

void Camera::setFov(float v) noexcept {
    if (!qFuzzyCompare(fov_, v)) {
        bool is_perspective_bak = isPerspective();
        fov_ = v;
        updateProjection();
        emit fovChanged(v);
        bool is_perspective = isPerspective();
        if (is_perspective != is_perspective_bak) {
            perspectiveChanged(is_perspective);
        }
    }
}

float Camera::aspectRatio() const noexcept {
    return aspect_ratio_;
}

void Camera::setAspectRatio(float v) noexcept {
    if (!qFuzzyCompare(aspect_ratio_, v)) {
        aspect_ratio_ = v;
        updateProjection();
        emit aspectRatioChanged(v);
    }
}

float Camera::nearPlane() const noexcept {
    return near_plane_;
}

void Camera::setNearPlane(float v) noexcept {
    if (v < near_plane_ && !qFuzzyCompare(near_plane_, v)) {
        near_plane_ = v;
        updateProjection();
        emit nearPlaneChanged(v);
    }
}

float Camera::farPlane() const noexcept {
    return far_plane_;
}

void Camera::setFarPlane(float v) noexcept {
    if (v > near_plane_ && !qFuzzyCompare(far_plane_, v)) {
        far_plane_ = v;
        updateProjection();
        emit farPlaneChanged(v);
    }
}

bool Camera::isPerspective() const noexcept {
    return (fov_ > 0.0f);
}

void Camera::lookTo(const QVector3D& pos, const QVector3D& direction, const QVector3D& up) noexcept {
    view_matrix_.lookAt(pos, pos + direction, up);
}

void Camera::lookAt(const QVector3D& pos, const QVector3D& center, const QVector3D& up) noexcept {
    view_matrix_.lookAt(pos, center, up);
}

void Camera::ortho(const QRectF& rect, float near_plane, float far_plane) noexcept {
    setFov(-1.0f);
    projection_matrix_.ortho(rect.left(), rect.right(), rect.bottom(), rect.top(), near_plane, far_plane);
}

void Camera::ortho(float left, float right, float top, float bottom, float near_plane, float far_plane) noexcept {
    setFov(-1.0f);
    projection_matrix_.ortho(left, right, bottom, top, near_plane, far_plane);
}

void Camera::perspective(float fov, float aspect_ratio, float near_plane, float far_plane) noexcept {
    fov_ = std::clamp(fov, 1.0f, 179.0f);
    aspect_ratio_ = aspect_ratio;
    near_plane_ = near_plane;
    far_plane_ = far_plane;
    updateProjection();
}

void Camera::updateProjection() noexcept {
    if (isPerspective()) {
        projection_matrix_.perspective(fov_, aspect_ratio_, near_plane_, far_plane_);
    } else {
        projection_matrix_.ortho(rect_.left(), rect_.right(), rect_.bottom(), rect_.top(), near_plane_, far_plane_);
    }
}


VOLCANO_WORLD_END
