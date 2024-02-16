//
//
#ifndef VOLCANO_FRAMEDATA_HPP
#define VOLCANO_FRAMEDATA_HPP

#include <QStack>

#include <Volcano/Common.hpp>
#include <Volcano/Transform.hpp>

VOLCANO_BEGIN

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

    Transform& transform() {
        return transform_stack_.top();
    }

    const Transform& transform() const {
        return transform_stack_.top();
    }

    void pushTransform() {
        transform_stack_.push(transform_stack_.top());
    }

    void pushTransform(const Transform& v) {
        transform_stack_.push(v);
    }

    void pushIdentityTransform() {
        transform_stack_.push(Transform());
    }

    void popTransform() {
        if (transform_stack_.size() > 1) {
            transform_stack_.pop();
        }
    }

    void buildModelMatrix(QMatrix4x4& v) const {
        transform_stack_.top().buildMatrix4x4(v);
    }

public:
    // add drawable objects.

private:
    QStack<Transform> transform_stack_;
    // drawable objects.
};

VOLCANO_END

#endif // VOLCANO_FRAMEDATA_HPP
