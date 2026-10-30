#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <limits>
#include <new>
#include <stdexcept>

namespace {
void* reallocateLayout(void* memory, std::size_t size) {
    auto* result = std::realloc(memory, size);
    if (!result) {
        throw std::bad_alloc();
    }
    return result;
}
} // namespace

#define LAY_REALLOC(memory, size) reallocateLayout(memory, size)
#define LAY_FREE(memory) std::free(memory)
#define LAY_IMPLEMENTATION
#include <layout.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Gui/Layout.h>

VOLCANO_GUI_BEGIN

void Layout::setLayout(Mode value) {
    layout_ = value;
    changed();
}

void Layout::setPadding(float value) {
    setPadding({value, value, value, value});
}

void Layout::setPadding(const Insets& value) {
    if (!std::isfinite(value.left) || !std::isfinite(value.top) || !std::isfinite(value.right) ||
        !std::isfinite(value.bottom) || value.left < 0 || value.top < 0 || value.right < 0 ||
        value.bottom < 0 || !std::isfinite(value.left + value.right) ||
        !std::isfinite(value.top + value.bottom)) {
        throw std::invalid_argument("Invalid GUI padding");
    }
    padding_ = value;
    changed();
}

void Layout::setGap(float value) {
    setGap(value, value);
}

void Layout::setGap(float horizontal, float vertical) {
    if (!std::isfinite(horizontal) || !std::isfinite(vertical) || horizontal < 0 || vertical < 0) {
        throw std::invalid_argument("Invalid GUI gap");
    }
    horizontal_gap_ = horizontal;
    vertical_gap_ = vertical;
    changed();
}

void Layout::setJustify(Justify value) {
    justify_ = value;
    changed();
}

void Layout::setAlignment(Alignment value) {
    VOLCANO_ASSERT(value != Alignment::Auto);
    alignment_ = value;
    changed();
}

void Layout::setWrap(bool value) {
    wrap_ = value;
    changed();
}

Sizef Layout::measure(Platform& platform, Sizef available) const {
    Widget::measure(platform, available);
    return calculate(platform, available, false);
}

void Layout::layout(const Rectf& bounds, Platform& platform) {
    setBounds(bounds);
    calculate(platform, {bounds.width, bounds.height}, true);
}

Sizef Layout::calculate(Platform& platform, Sizef available, bool arrange) const {
    const bool widthKnown =
        arrange || widthSizing() != Sizing::Auto || (wrap_ && layout_ == Mode::Horizontal);
    const bool heightKnown =
        arrange || heightSizing() != Sizing::Auto || (wrap_ && layout_ == Mode::Vertical);
    const float width = !arrange && widthSizing() == Sizing::Fixed ? frame().width : available.width;
    const float height = !arrange && heightSizing() == Sizing::Fixed ? frame().height : available.height;
    const float paddingWidth = padding_.left + padding_.right;
    const float paddingHeight = padding_.top + padding_.bottom;
    const Sizef content{std::max(0.0f, width - paddingWidth), std::max(0.0f, height - paddingHeight)};

    std::vector<Widget*> active;
    for (const auto& child : children()) {
        if (child->visible()) {
            active.push_back(child);
        } else if (arrange) {
            child->layout({bounds().left, bounds().top, 0, 0}, platform);
        }
    }
    if (arrange && (content.width == 0 || content.height == 0)) {
        for (auto* child : active) {
            child->layout({bounds().left, bounds().top, 0, 0}, platform);
        }
        return available;
    }
    if (active.size() >= std::numeric_limits<lay_id>::max()) {
        throw std::length_error("Too many GUI layout items");
    }

    lay_context context;
    lay_init_context(&context);
    auto cleanup = scopeGuard([&] {
        lay_destroy_context(&context);
    });
    lay_reserve_items_capacity(&context, static_cast<lay_id>(active.size() + 1));
    const auto root = lay_item(&context);
    std::uint32_t contain = LAY_LAYOUT;
    if (layout_ != Mode::Absolute) {
        contain = layout_ == Mode::Horizontal ? LAY_ROW : LAY_COLUMN;
        if (wrap_) {
            contain |= LAY_WRAP;
        }
        switch (justify_) {
        case Justify::Start:
            contain |= LAY_START;
            break;
        case Justify::Center:
            contain |= LAY_MIDDLE;
            break;
        case Justify::End:
            contain |= LAY_END;
            break;
        case Justify::SpaceBetween:
            contain |= !wrap_ && active.size() > 1 ? LAY_JUSTIFY : LAY_START;
            break;
        }
    }
    lay_set_contain(&context, root, contain);
    const float gapX =
        layout_ == Mode::Horizontal || (layout_ == Mode::Vertical && wrap_) ? horizontal_gap_ : 0;
    const float gapY =
        layout_ == Mode::Vertical || (layout_ == Mode::Horizontal && wrap_) ? vertical_gap_ : 0;
    // Each child carries a trailing gap; extend the root to avoid a trailing outer gap.
    const float rootWidth =
        widthKnown ? std::max(std::numeric_limits<float>::min(), content.width + gapX) : 0;
    const float rootHeight =
        heightKnown ? std::max(std::numeric_limits<float>::min(), content.height + gapY) : 0;
    if (!std::isfinite(rootWidth) || !std::isfinite(rootHeight)) {
        throw std::overflow_error("GUI layout size overflow");
    }
    lay_set_size_xy(&context, root, rootWidth, rootHeight);

    std::vector<Sizef> measured;
    measured.reserve(active.size());
    lay_id previous = LAY_INVALID_ID;
    for (auto* child : active) {
        const auto& margin = child->margin();
        const float left = margin.left + child->frame().left;
        const float top = margin.top + child->frame().top;
        const float right = margin.right + gapX;
        const float bottom = margin.bottom + gapY;
        if (!std::isfinite(left) || !std::isfinite(top) || !std::isfinite(right) || !std::isfinite(bottom) ||
            !std::isfinite(left + right) || !std::isfinite(top + bottom)) {
            throw std::overflow_error("GUI layout margin overflow");
        }
        const Sizef childAvailable{std::max(0.0f, content.width - left - margin.right),
                                   std::max(0.0f, content.height - top - margin.bottom)};
        const auto preferred = child->measure(platform, childAvailable);
        if (!std::isfinite(preferred.width) || !std::isfinite(preferred.height) || preferred.width < 0 ||
            preferred.height < 0) {
            throw std::runtime_error("GUI widget returned an invalid preferred size");
        }
        measured.push_back(preferred);
        const auto item = lay_item(&context);
        if (previous == LAY_INVALID_ID) {
            lay_insert(&context, root, item);
        } else {
            lay_append(&context, previous, item);
        }
        previous = item;
        lay_set_size_xy(&context, item, preferred.width, preferred.height);
        auto* data = lay_get_item(&context, item);
        if (child->widthSizing() != Sizing::Fixed) {
            data->flags &= ~LAY_ITEM_HFIXED;
        } else {
            data->flags |= LAY_ITEM_HFIXED;
        }
        if (child->heightSizing() != Sizing::Fixed) {
            data->flags &= ~LAY_ITEM_VFIXED;
        } else {
            data->flags |= LAY_ITEM_VFIXED;
        }
        const auto horizontal =
            child->horizontalAlignment() == Alignment::Auto ? alignment_ : child->horizontalAlignment();
        const auto vertical =
            child->verticalAlignment() == Alignment::Auto ? alignment_ : child->verticalAlignment();
        std::uint32_t behave = horizontal == Alignment::End      ? LAY_RIGHT
                               : horizontal == Alignment::Center ? LAY_HCENTER
                                                                 : LAY_LEFT;
        behave |= vertical == Alignment::End      ? LAY_BOTTOM
                  : vertical == Alignment::Center ? LAY_VCENTER
                                                  : LAY_TOP;
        if (child->widthSizing() == Sizing::Fill) {
            behave = (behave & ~LAY_HFILL) | LAY_HFILL;
        }
        if (child->heightSizing() == Sizing::Fill) {
            behave = (behave & ~LAY_VFILL) | LAY_VFILL;
        }
        lay_set_behave(&context, item, behave);
        lay_set_margins_ltrb(&context, item, left, top, right, bottom);
    }
    const auto run = [&] {
        for (lay_id item = 1; item <= active.size(); ++item) {
            lay_clear_item_break(&context, item);
            if (wrap_ && active[item - 1]->breakBefore()) {
                lay_get_item(&context, item)->flags |= LAY_BREAK;
            }
        }
        lay_run_context(&context);
        for (lay_id item = 0; item <= active.size(); ++item) {
            const auto rect = lay_get_rect(&context, item);
            if (!std::isfinite(rect[0]) || !std::isfinite(rect[1]) || !std::isfinite(rect[2]) ||
                !std::isfinite(rect[3])) {
                throw std::overflow_error("GUI layout result overflow");
            }
        }
    };
    run();

    bool reflow = false;
    if (!widthKnown && lay_get_rect(&context, root)[2] > content.width + gapX) {
        lay_set_size_xy(&context, root, std::max(std::numeric_limits<float>::min(), content.width + gapX),
                        rootHeight);
        reflow = true;
    }
    if (reflow) {
        run();
    }
    reflow = false;
    for (std::size_t i = 0; i < active.size(); ++i) {
        const auto item = static_cast<lay_id>(i + 1);
        const auto rect = lay_get_rect(&context, item);
        if (active[i]->heightSizing() != Sizing::Fixed && rect[2] != measured[i].width) {
            const auto preferred =
                active[i]->measure(platform, {std::max(0.0f, rect[2]), std::max(0.0f, rect[3])});
            if (!std::isfinite(preferred.height) || preferred.height < 0) {
                throw std::runtime_error("GUI widget returned an invalid preferred height");
            }
            if (preferred.height != measured[i].height) {
                lay_get_item(&context, item)->size[1] = preferred.height;
                reflow = true;
            }
        }
    }
    if (reflow) {
        run();
    }
    // layout.h divides by (line item count - 1) for wrapped justification, even for one item.
    if (wrap_ && layout_ != Mode::Absolute && justify_ == Justify::SpaceBetween) {
        const int axis = layout_ == Mode::Horizontal ? 0 : 1;
        std::size_t start = 0;
        while (start < active.size()) {
            auto end = start + 1;
            while (end < active.size() &&
                   !(lay_get_item(&context, static_cast<lay_id>(end + 1))->flags & LAY_BREAK)) {
                ++end;
            }
            if (end < active.size() && end - start > 1 && !active[end]->breakBefore()) {
                const auto last = static_cast<lay_id>(end);
                const auto rect = lay_get_rect(&context, last);
                const float used =
                    rect[axis] + rect[axis + 2] + lay_get_item(&context, last)->margins[axis + 2];
                const float extra = std::max(0.0f, lay_get_rect(&context, root)[axis + 2] - used);
                const float spacing = extra / static_cast<float>(end - start - 1);
                for (auto i = start + 1; i < end; ++i) {
                    context.rects[i + 1][axis] += spacing * static_cast<float>(i - start);
                }
            }
            start = end;
        }
    }
    if (arrange) {
        for (std::size_t i = 0; i < active.size(); ++i) {
            const auto item = static_cast<lay_id>(i + 1);
            auto rect = lay_get_rect(&context, item);
            const auto* data = lay_get_item(&context, item);
            // Center the margin box; layout.h otherwise applies the full margin difference.
            if (layout_ != Mode::Horizontal && (data->flags & LAY_HFILL) == LAY_HCENTER) {
                rect[0] += (data->margins[2] - data->margins[0]) * 0.5f;
            }
            if (layout_ != Mode::Vertical && (data->flags & LAY_VFILL) == LAY_VCENTER) {
                rect[1] += (data->margins[3] - data->margins[1]) * 0.5f;
            }
            active[i]->layout({bounds().left + padding_.left + rect[0], bounds().top + padding_.top + rect[1],
                               std::max(0.0f, rect[2]), std::max(0.0f, rect[3])},
                              platform);
        }
        return available;
    }
    const auto result = lay_get_rect(&context, root);
    const Sizef preferred{
        widthKnown ? width : std::max(0.0f, result[2] - (active.empty() ? 0 : gapX)) + paddingWidth,
        heightKnown ? height : std::max(0.0f, result[3] - (active.empty() ? 0 : gapY)) + paddingHeight};
    if (!std::isfinite(preferred.width) || !std::isfinite(preferred.height)) {
        throw std::overflow_error("GUI preferred size overflow");
    }
    return preferred;
}

VOLCANO_GUI_END
