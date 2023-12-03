//
//
#ifndef VOLCANO_VIEW_HPP
#define VOLCANO_VIEW_HPP

#include <QStack>
#include <QObject>

#include <Volcano/Common.hpp>
#include <Volcano/Transform.hpp>

VOLCANO_BEGIN

class View: public QObject {
public:
    View(QObject* parent = nullptr);

public:
    virtual void beginFrame() {
        transform_stack_.clear();
        transform_stack_.push(Transform::identity());
    }

    virtual void endFrame() {
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

    Transform& currentTransform() {
        return transform_stack_.top();
    }

    const Transform& currentTransform() const {
        return transform_stack_.top();
    }

    void pushTransform() {
        transform_stack_.push(transform_stack_.top());
    }

    void pushTransform(const Transform& v) {
        transform_stack_.push(v);
    }

    void pushIdentityTransform() {
        transform_stack_.push(Transform::identity());
    }

    void popTransform() {
        if (transform_stack_.size() > 1) {
            transform_stack_.pop();
        }
    }

    void buildModelMatrix(QMatrix4x4& v) const {
        currentTransform().buildMatrix4x4(v);
    }

private:
    QVector3D eye_;
    QVector3D center_;
    QVector3D up_;
    float vertical_fov_;
    float aspect_rate_;
    float near_plane_;
    float far_plane_;
    QVector4D viewport_;
    QStack<Transform> transform_stack_;
};

VOLCANO_END

#endif // VOLCANO_VIEW_HPP
