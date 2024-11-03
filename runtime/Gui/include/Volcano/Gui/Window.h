//
//
#ifndef VOLCANO_GUI_WINDOW_H
#define VOLCANO_GUI_WINDOW_H

#include <string>

#include <Volcano/Color.h>
#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Rect.h>
#include <Volcano/Gui/Context.h>

VOLCANO_GUI_BEGIN

class Window {
public:
    enum {
        FlagFrameLess = 0x1,
        FlagMovable = 0x2,
        FlagResizable = 0x4
    };

public:
    Window(const std::string& title, Rect rect, int flags = FlagMovable | FlagResizable);
    virtual ~Window() = default;

public:
    virtual void update();

    const std::string& title() const {
        return title_;
    }

    void setTitle(const std::string& v) {
        title_ = v;
    }

    void setTitle(std::string&& v) {
        title_ = std::move(v);
    }

    float x() const {
        return rect_.x();
    }

    void setX(float v) {
        rect_.setX(v);
    }

    float y() const {
        return rect_.y();
    }

    void setY(float v) {
        rect_.setY(v);
    }

    Point pos() const {
        return rect_.pos();
    }

    void setPos(int x, int y) {
        rect_.setPos(x, y);
    }

    void setPos(Point v) {
        rect_.setPos(v);
    }

    void setPos(Eigen::Vector2f v) {
        rect_.setPos(v);
    }

    float width() const {
        return rect_.width();
    }

    void setWidth(float v) {
        rect_.setWidth(v);
    }

    float height() const {
        return rect_.height();
    }

    void setHeight(float v) {
        rect_.setHeight(v);
    }

    Size size() const {
        return rect_.size();
    }

    void setSize(float w, float h) {
        rect_.setSize(w, h);
    }

    void setSize(Size v) {
        rect_.setSize(v);
    }

    void setSize(Eigen::Vector2f v) {
        rect_.setSize(v);
    }

    Rect rect() const {
        return rect_;
    }

    void setRect(Rect v) {
        rect_ = v;
    }

private:
    int flags_;
    std::string title_;
    Rect rect_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WINDOW_H
