//
//
#ifndef VOLCANO_GRAPHICS_CAMERA_H
#define VOLCANO_GRAPHICS_CAMERA_H

#include <QRectF>
#include <QVector3D>
#include <QMatrix4x4>

#include <Volcano/World/Transformable.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Camera: public World::Transformable {
    Q_OBJECT
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged FINAL)
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged FINAL)
    Q_PROPERTY(float aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY aspectRatioChanged FINAL)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged FINAL)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged FINAL)

public:
    Camera(QObject* parent = nullptr);

public:
    void reset() {
        view_.setToIdentity();
        projection_.setToIdentity();
    }

    const QRectF& rect() const {
        return rect_;
    }

    void setRect(const QRectF& v) {
        if (!qFuzzyCompare(rect_, v)) {
            rect_ = v;
            emit rectChanged(v);
        }
    }

    QMatrix4x4& view() {
		return view_;
	}

    const QMatrix4x4& view() const {
		return view_;
	}

    void lookTo(QVector3D pos, QVector3D direction, QVector3D up) {
        lookAt(pos, direction.normalized(), up);
	}

    void lookAt(QVector3D pos, QVector3D center, QVector3D up) {
        view_.lookAt(pos, center, up);
	}

    QMatrix4x4& projection() {
        return projection_;
    }

    const QMatrix4x4& projection() const {
        return projection_;
    }

    void ortho(const QRectF& rect, float near_plane, float far_plane) {
        projection_.ortho(rect.left(), rect.right(), rect.bottom(), rect.top(), near_plane, far_plane);
    }

    void ortho(float left, float right, float top, float bottom, float near_plane, float far_plane) {
        projection_.ortho(left, right, bottom, top, near_plane, far_plane);
	}

    float fov() const {
        return fov_;
    }

    void setFov(float v) {
        if (!qFuzzyCompare(fov_, v)) {
            fov_ = v;
            updateProjection();
            emit fovChanged(v);
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
            emit nearPlaneChanged(v);
        }
    }

    void perspective(float fov, float aspect_ratio, float near_plane, float far_plane) {
        projection_.perspective(fov, aspect_ratio, near_plane, far_plane);
        fov_ = fov;
        aspect_ratio_ = aspect_ratio;
        near_plane_ = near_plane;
        far_plane_ = far_plane;
	}

signals:
    void rectChanged(const QRectF& v);
    void fovChanged(float v);
    void aspectRatioChanged(float v);
    void nearPlaneChanged(float v);
    void farPlaneChanged(float v);

private:
    void updateProjection() {
        if (fov_ > 0.0f) {
            projection_.ortho(rect_);
        } else {
            projection_.perspective(fov_, aspect_ratio_, near_plane_, far_plane_);
        }
    }

private:
    QMatrix4x4 view_;
    QMatrix4x4 projection_;
    QRectF rect_;
    float fov_;
    float aspect_ratio_;
    float near_plane_;
    float far_plane_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERA_H
