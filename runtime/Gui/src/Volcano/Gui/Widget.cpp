#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Gui/Widget.h>

VOLCANO_GUI_BEGIN

Widget& Widget::root() noexcept {
    auto* widget = this;
    while (widget->parent_) {
        widget = widget->parent_;
    }
    return *widget;
}

const Widget& Widget::root() const noexcept {
    auto* widget = this;
    while (widget->parent_) {
        widget = widget->parent_;
    }
    return *widget;
}

bool Widget::contains(const Widget& widget) const noexcept {
    for (auto* current = &widget; current; current = current->parent_) {
        if (current == this) {
            return true;
        }
    }
    return false;
}

void Widget::observe(Observer& observer) {
    VOLCANO_ASSERT(parent_ == nullptr && observer_ == nullptr && !changing_tree_);
    observer_ = &observer;
}

void Widget::unobserve(Observer& observer) noexcept {
    VOLCANO_ASSERT(observer_ == &observer && !changing_tree_ && !observer.busy());
    observer_ = nullptr;
}

void Widget::checkTreeChange() const {
    const auto& tree = root();
    VOLCANO_ASSERT(!tree.changing_tree_ && (!tree.observer_ || !tree.observer_->busy()));
}

Widget::Widget(Widget* parent) {
    setParent(parent);
}

Widget::~Widget() {
    checkTreeChange();
    auto& tree = root();
    if (tree.observer_) {
        tree.observer_->destroy(*this);
    }
    if (parent_) {
        auto& siblings = parent_->children_;
        siblings.erase(std::find(siblings.begin(), siblings.end(), this));
        parent_->changed();
        parent_ = nullptr;
    }
    while (!children_.empty()) {
        auto* child = children_.back();
        children_.pop_back();
        child->parent_ = nullptr;
        delete child;
    }
}

void Widget::setParent(Widget* parent) {
    if (parent == parent_) {
        return;
    }
    checkTreeChange();
    VOLCANO_ASSERT(observer_ == nullptr && (!parent || !contains(*parent)));
    if (parent) {
        parent->checkTreeChange();
        if (parent->children_.size() == parent->children_.capacity()) {
            parent->children_.reserve(std::max(std::size_t{1}, parent->children_.capacity() * 2));
        }
    }
    auto& tree = root();
    auto* destination = parent ? &parent->root() : nullptr;
    tree.changing_tree_ = true;
    if (destination) {
        destination->changing_tree_ = true;
    }
    auto restore = scopeGuard([&tree, destination] {
        tree.changing_tree_ = false;
        if (destination) {
            destination->changing_tree_ = false;
        }
    });
    if (tree.observer_) {
        tree.observer_->remove(*this);
    }
    if (parent_) {
        auto& siblings = parent_->children_;
        siblings.erase(std::find(siblings.begin(), siblings.end(), this));
        parent_->changed();
    }
    parent_ = parent;
    if (parent_) {
        parent_->children_.push_back(this);
    }
    changed();
}

void Widget::changed() noexcept {
    for (auto* widget = this; widget; widget = widget->parent_) {
        ++widget->revision_;
    }
}

void Widget::setFrame(const Rectf& value) {
    if (!std::isfinite(value.left) || !std::isfinite(value.top) || !std::isfinite(value.width) ||
        !std::isfinite(value.height) || value.width < 0 || value.height < 0) {
        throw std::invalid_argument("Invalid widget frame");
    }
    frame_ = value;
    width_sizing_ = Sizing::Fixed;
    height_sizing_ = Sizing::Fixed;
    changed();
}

void Widget::setWidth(float value) {
    if (!std::isfinite(value) || value < 0) {
        throw std::invalid_argument("Invalid widget width");
    }
    frame_.width = value;
    width_sizing_ = Sizing::Fixed;
    changed();
}

void Widget::setHeight(float value) {
    if (!std::isfinite(value) || value < 0) {
        throw std::invalid_argument("Invalid widget height");
    }
    frame_.height = value;
    height_sizing_ = Sizing::Fixed;
    changed();
}

void Widget::setWidth(Sizing value) {
    width_sizing_ = value;
    changed();
}

void Widget::setHeight(Sizing value) {
    height_sizing_ = value;
    changed();
}

void Widget::setMargin(float value) {
    setMargin({value, value, value, value});
}

void Widget::setMargin(const Insets& value) {
    if (!std::isfinite(value.left) || !std::isfinite(value.top) || !std::isfinite(value.right) ||
        !std::isfinite(value.bottom)) {
        throw std::invalid_argument("Invalid widget margin");
    }
    margin_ = value;
    changed();
}

void Widget::setHorizontalAlignment(Alignment value) {
    horizontal_alignment_ = value;
    changed();
}

void Widget::setVerticalAlignment(Alignment value) {
    vertical_alignment_ = value;
    changed();
}

void Widget::setBreakBefore(bool value) {
    break_before_ = value;
    changed();
}

void Widget::setVisible(bool value) {
    if (visible_ != value) {
        visible_ = value;
        changed();
    }
}

void Widget::setEnabled(bool value) {
    if (enabled_ != value) {
        enabled_ = value;
        changed();
    }
}

void Widget::setBackground(const Colorf& value) {
    background_ = value;
    changed();
}

Sizef Widget::measure(Platform&, Sizef available) const {
    if (!std::isfinite(available.width) || !std::isfinite(available.height) || available.width < 0 ||
        available.height < 0) {
        throw std::invalid_argument("Invalid GUI measure size");
    }
    return {width_sizing_ == Sizing::Fixed ? frame_.width : 0,
            height_sizing_ == Sizing::Fixed ? frame_.height : 0};
}

void Widget::setBounds(const Rectf& bounds) {
    if (!std::isfinite(bounds.left) || !std::isfinite(bounds.top) || !std::isfinite(bounds.width) ||
        !std::isfinite(bounds.height) || !std::isfinite(bounds.left + bounds.width) ||
        !std::isfinite(bounds.top + bounds.height) || bounds.width < 0 || bounds.height < 0) {
        throw std::invalid_argument("Invalid GUI layout bounds");
    }
    bounds_ = bounds;
}

void Widget::layout(const Rectf& bounds, Platform& platform) {
    setBounds(bounds);
    for (const auto& child : children_) {
        if (!child->visible() || bounds.empty()) {
            child->layout({bounds.left, bounds.top, 0, 0}, platform);
            continue;
        }
        const auto& margin = child->margin();
        const auto& frame = child->frame();
        const float left = frame.left + margin.left;
        const float top = frame.top + margin.top;
        const Sizef available{std::max(0.0f, bounds.width - left - margin.right),
                              std::max(0.0f, bounds.height - top - margin.bottom)};
        const auto preferred = child->measure(platform, available);
        const float width = child->widthSizing() == Sizing::Fill ? available.width : preferred.width;
        const float height = child->heightSizing() == Sizing::Fill ? available.height : preferred.height;
        child->layout({bounds.left + left, bounds.top + top, width, height}, platform);
    }
}

void Widget::paint(World::Canvas& canvas, Platform& platform) const {
    const auto& color = background(platform);
    if (color.alpha() != 0) {
        canvas.setColor(color);
        canvas.beginPath();
        canvas.rect(bounds_);
        canvas.fill();
    }
}

void Widget::draw(World::Canvas& canvas, Platform& platform) const {
    if (!visible_) {
        return;
    }
    paint(canvas, platform);
    if (!children_.empty()) {
        canvas.save();
        auto restore = scopeGuard([&canvas] {
            canvas.restore();
        });
        canvas.setClip(bounds_);
        for (const auto& child : children_) {
            child->draw(canvas, platform);
        }
    }
    const auto& style = platform.style();
    if (style.borderWidth() > 0 && style.borderColor().alpha() != 0 && !bounds_.empty()) {
        const float width = std::min({style.borderWidth(), bounds_.width, bounds_.height});
        const float inset = width * 0.5f;
        canvas.setColor(style.borderColor());
        canvas.beginPath();
        canvas.rect(
            {bounds_.left + inset, bounds_.top + inset, bounds_.width - width, bounds_.height - width});
        canvas.stroke(width);
    }
}

Widget* Widget::hit(float x, float y) {
    if (!visible_ || !enabled_ || !bounds_.contains(x, y)) {
        return nullptr;
    }
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if (auto* hit = (*it)->hit(x, y)) {
            return hit;
        }
    }
    return this;
}

bool Widget::acceptsInput(const Widget& target) const noexcept {
    for (auto* widget = &target; widget; widget = widget->parent_) {
        if (!widget->visible_ || !widget->enabled_) {
            return false;
        }
        if (widget == this) {
            return true;
        }
    }
    return false;
}

bool Widget::event(const PointerEvent&, Platform&) {
    return false;
}

bool Widget::event(const ScrollEvent&, Platform&) {
    return false;
}

bool Widget::event(const KeyEvent&, Platform&) {
    return false;
}

bool Widget::event(const TextInputEvent&, Platform&) {
    return false;
}

bool Widget::event(const CompositionEvent&, Platform&) {
    return false;
}

bool Widget::event(const FocusEvent&, Platform&) {
    return false;
}

void Widget::collectResources(std::vector<std::filesystem::path>& fonts,
                              std::vector<std::string>& images) const {
    for (const auto& child : children_) {
        child->collectResources(fonts, images);
    }
}

VOLCANO_GUI_END
