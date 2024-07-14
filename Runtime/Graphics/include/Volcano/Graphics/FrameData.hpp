//
//
#ifndef VOLCANO_GRAPHICS_FRAMEDATA_HPP
#define VOLCANO_GRAPHICS_FRAMEDATA_HPP

#include <QStack>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class FrameData final {
public:
    FrameData() {
        transform_stack_.emplaceBack();
    }

    FrameData(const FrameData& other)
        : transform_stack_(other.transform_stack_) {
    }

    FrameData(FrameData&& other)
        : transform_stack_(std::move(other.transform_stack_)) {
    }

public:
    FrameData& operator=(const FrameData& other) {
        if (this != &other) {
            transform_stack_ = other.transform_stack_;
        }
        return (*this);
    };

    FrameData& operator=(FrameData&& other) {
        transform_stack_ = std::move(other.transform_stack_);
        return (*this);
    }

public:
    void reset() {
        transform_stack_.clear();
        transform_stack_.emplaceBack();
    }

    Eigen::Affine3f& transform() {
        return transform_stack_.top();
    }

    const Eigen::Affine3f& transform() const {
        return transform_stack_.top();
    }

    void pushTransform() {
        transform_stack_.push(transform_stack_.top());
    }

    void pushTransform(const Eigen::Affine3f& v) {
        transform_stack_.push(v);
    }

    void pushIdentityTransform() {
        transform_stack_.push(Eigen::Affine3f());
    }

    void popTransform() {
        if (transform_stack_.size() > 1) {
            transform_stack_.pop();
        }
    }

    auto modelMatrix() const {
        transform_stack_.top().matrix();
    }

public:
    // add drawable objects.

private:
    QStack<Eigen::Affine3f> transform_stack_;
    // drawable objects.
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_FRAMEDATA_HPP
