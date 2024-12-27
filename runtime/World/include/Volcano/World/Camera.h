//
//
#ifndef VOLCANO_WORLD_CAMERA_H
#define VOLCANO_WORLD_CAMERA_H

#include <QRect>
#include <QMatrix4x4>

#include <Volcano/World/Common.h>
#include <Volcano/World/Transformable.h>

VOLCANO_WORLD_BEGIN

class Camera: public Transformable {
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged FINAL)
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged FINAL)
    Q_PROPERTY(float aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged FINAL)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged FINAL)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged FINAL)
    Q_PROPERTY(bool perspective READ isPerspective NOTIFY perspectiveChanged FINAL)

public:
    Camera(QObject* parent = nullptr);

public:
    void reset() {
        view_matrix_.setToIdentity();
        projection_matrix_.setToIdentity();
    }

    const QMatrix4x4& viewMatrix() const {
        return view_matrix_;
    }

    const QMatrix4x4& projectionMatrix() const {
        return projection_matrix_;
    }

    const QRectF& rect() const {
        return rect_;
    }

    void setRect(const QRectF& v) {
        if (!qFuzzyCompare(rect_, v)) {
            rect_ = v.normalized();
            emit rectChanged(rect_);
        }
    }

    float fov() const {
        return fov_;
    }

    void setFov(float v) {
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

    float aspectRatio() const {
        return aspect_ratio_;
    }

    void setAspectRatio(float v) {
        if (!qFuzzyCompare(aspect_ratio_, v)) {
            aspect_ratio_ = v;
            updateProjection();
            emit aspectRatioChanged(v);
        }
    }

    float nearPlane() const {
        return near_plane_;
    }

    void setNearPlane(float v) {
        if (v < near_plane_ && !qFuzzyCompare(near_plane_, v)) {
            near_plane_ = v;
            updateProjection();
            emit nearPlaneChanged(v);
        }
    }

    float farPlane() const {
        return far_plane_;
    }

    void setFarPlane(float v) {
        if (v > near_plane_ && !qFuzzyCompare(far_plane_, v)) {
            far_plane_ = v;
            updateProjection();
            emit farPlaneChanged(v);
        }
    }

    bool isPerspective() const {
        return (fov_ > 0.0f);
    }

    Q_INVOKABLE void lookTo(const QVector3D& pos, const QVector3D& direction, const QVector3D& up) {
        view_matrix_.lookAt(pos, pos + direction, up);
	}

    Q_INVOKABLE void lookAt(const QVector3D& pos, const QVector3D& center, const QVector3D& up) {
        view_matrix_.lookAt(pos, center, up);
	}

    Q_INVOKABLE void ortho(const QRectF& rect, float near_plane, float far_plane) {
        setFov(-1.0f);
        projection_matrix_.ortho(rect.left(), rect.right(), rect.bottom(), rect.top(), near_plane, far_plane);
    }

    Q_INVOKABLE void ortho(float left, float right, float top, float bottom, float near_plane, float far_plane) {
        setFov(-1.0f);
        projection_matrix_.ortho(left, right, bottom, top, near_plane, far_plane);
	}

    void perspective(float fov, float aspect_ratio, float near_plane, float far_plane) {
        fov_ = std::clamp(fov, 1.0f, 179.0f);
        aspect_ratio_ = aspect_ratio;
        near_plane_ = near_plane;
        far_plane_ = far_plane;
        updateProjection();
	}

signals:
    void rectChanged(const QRectF& v);
    void fovChanged(float v);
    void aspectRatioChanged(float v);
    void nearPlaneChanged(float v);
    void farPlaneChanged(float v);
    void perspectiveChanged(bool v);

private:
    void updateProjection() {
        if (isPerspective()) {
            projection_matrix_.perspective(fov_, aspect_ratio_, near_plane_, far_plane_);
        } else {
            projection_matrix_.ortho(rect_.left(), rect_.right(), rect_.bottom(), rect_.top(), near_plane_, far_plane_);
        }
    }

private:
    QMatrix4x4 view_matrix_;
    QMatrix4x4 projection_matrix_;
    QRectF rect_;
    float fov_;
    float aspect_ratio_;
    float near_plane_;
    float far_plane_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CAMERA_H
