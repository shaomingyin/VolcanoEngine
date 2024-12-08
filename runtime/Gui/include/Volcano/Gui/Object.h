//
//
#ifndef VOLCANO_GUI_OBJECT_H
#define VOLCANO_GUI_OBJECT_H

#include <string>
#include <set>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

class Object {
public:
    Object(Object* parent = nullptr);
    Object(Object&&) = default;
    Object& operator=(Object&&) = default;
    virtual ~Object();

public:
    bool isEnabled() const {
        return enabled_;
    }

    void enable() {
        enabled_ = true;
    }

    void disable() {
        enabled_ = false;
    }

    bool* enablement() {
        return &enabled_;
    }

    bool isVisible() const {
        return visible_;
    }

    void show() {
        visible_ = true;
    }

    void hide() {
        visible_ = false;
    }

    bool* visiblility() {
        return &visible_;
    }

    Object* parent() {
        return parent_;
    }

    void setParent(Object* p);
    void update();

protected:
    void updateChidlren() {
        for (auto& child : children_) {
            child->update();
        }
    }

    virtual void paint() = 0;

private:
    void addChild(Object* p) {
        VOLCANO_ASSERT(p != nullptr);
        children_.insert(p);
    }

    void removeChild(Object* p) {
        VOLCANO_ASSERT(p != nullptr);
        children_.erase(p);
    }

private:
    Object* parent_;
    std::set<Object*> children_;
    bool enabled_;
    bool visible_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_OBJECT_H
