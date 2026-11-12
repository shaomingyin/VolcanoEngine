#include <cfenv>
#include <limits>
#include <memory>
#include <stdexcept>

#include <gtest/gtest.h>
#include "Platform.h"

#include <Volcano/Gui/Button.h>
#include <Volcano/Gui/Layout.h>
#include <Volcano/Gui/Document.h>
#include <Volcano/Gui/Text.h>
#include <Volcano/Gui/Widget.h>

namespace {
using namespace Volcano;
using Sizing = Gui::Widget::Sizing;
using Alignment = Gui::Widget::Alignment;

TEST(GuiLayout, RowPaddingMarginsGapAndEqualFill) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Horizontal);
    root->setPadding(10);
    root->setGap(5);
    auto* first = new Gui::Widget(root.get());
    auto* a = first;
    a->setWidth(20);
    a->setHeight(10);
    a->setMargin({2, 3, 4, 5});

    auto* second = new Gui::Widget(root.get());
    auto* b = second;
    b->setWidth(Sizing::Fill);
    b->setHeight(Sizing::Fill);

    auto* third = new Gui::Widget(root.get());
    auto* c = third;
    c->setWidth(Sizing::Fill);
    c->setHeight(10);

    Gui::Document document(platform, std::move(root));
    document.layout({120, 60});
    EXPECT_EQ(a->bounds(), (Rectf{12, 13, 20, 10}));
    EXPECT_EQ(b->bounds(), (Rectf{41, 10, 32, 40}));
    EXPECT_EQ(c->bounds(), (Rectf{78, 10, 32, 10}));
    EXPECT_EQ(a->frame().width, 20);
    document.layout({160, 60});
    EXPECT_FLOAT_EQ(b->bounds().width, 52);
    EXPECT_FLOAT_EQ(c->bounds().width, 52);
}

TEST(GuiLayout, AbsoluteAlignmentCentersMarginBoxAndPreservesOffsets) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setPadding({10, 20, 30, 40});
    auto* child = new Gui::Widget(root.get());
    auto* control = child;
    control->setFrame({0, 0, 40, 20});
    control->setMargin({3, 5, 7, 9});
    control->setHorizontalAlignment(Alignment::End);
    control->setVerticalAlignment(Alignment::Center);

    Gui::Document document(platform, std::move(root));
    document.layout({200, 160});
    EXPECT_EQ(control->bounds(), (Rectf{123, 58, 40, 20}));
    control->setHorizontalAlignment(Alignment::Center);
    EXPECT_NO_THROW(document.layout({200, 160}));
    EXPECT_FLOAT_EQ(control->bounds().left, 68);
    control->setWidth(Sizing::Fill);
    control->setHeight(Sizing::Fill);
    document.layout({200, 160});
    EXPECT_EQ(control->bounds(), (Rectf{13, 25, 150, 86}));
}

TEST(GuiLayout, MainAxisJustifyAndCrossAxisAlignment) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Horizontal);
    root->setJustify(Gui::Layout::Justify::SpaceBetween);
    root->setAlignment(Alignment::Center);
    auto* first = new Gui::Widget(root.get());
    auto* a = first;
    a->setFrame({0, 0, 20, 10});

    auto* second = new Gui::Widget(root.get());
    auto* b = second;
    b->setFrame({0, 0, 30, 20});
    b->setVerticalAlignment(Alignment::End);

    Gui::Document document(platform, std::move(root));
    document.layout({100, 50});
    EXPECT_EQ(a->bounds(), (Rectf{0, 20, 20, 10}));
    EXPECT_EQ(b->bounds(), (Rectf{70, 30, 30, 20}));
    document.root().setJustify(Gui::Layout::Justify::Center);
    document.layout({100, 50});
    EXPECT_FLOAT_EQ(a->bounds().left, 25);
    EXPECT_FLOAT_EQ(b->bounds().left, 45);
    document.root().setJustify(Gui::Layout::Justify::End);
    document.layout({100, 50});
    EXPECT_FLOAT_EQ(a->bounds().left, 50);
    EXPECT_FLOAT_EQ(b->bounds().left, 70);
}

TEST(GuiLayout, WrapsWithSeparateAxisGapsAndRecomputesAfterResize) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Horizontal);
    root->setWrap(true);
    root->setGap(5, 7);
    std::vector<Gui::Widget*> controls;
    for (int i = 0; i < 3; ++i) {
        auto* child = new Gui::Widget(root.get());
        child->setFrame({0, 0, 30, 10});
        controls.push_back(child);
    }
    Gui::Document document(platform, std::move(root));
    document.layout({70, 100});
    EXPECT_EQ(controls[0]->bounds(), (Rectf{0, 0, 30, 10}));
    EXPECT_EQ(controls[1]->bounds(), (Rectf{35, 0, 30, 10}));
    EXPECT_EQ(controls[2]->bounds(), (Rectf{0, 17, 30, 10}));
    document.layout({110, 100});
    EXPECT_EQ(controls[2]->bounds(), (Rectf{70, 0, 30, 10}));
    controls[1]->setBreakBefore(true);
    document.layout({110, 100});
    EXPECT_EQ(controls[1]->bounds(), (Rectf{0, 17, 30, 10}));
    EXPECT_EQ(controls[2]->bounds(), (Rectf{35, 17, 30, 10}));
}

TEST(GuiLayout, VerticalWrapAndNestedAutoSize) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Vertical);
    root->setWrap(true);
    root->setGap(4, 5);
    auto* nested = new Gui::Layout(root.get());
    auto* panel = nested;
    nested->setLayout(Gui::Layout::Mode::Horizontal);
    nested->setPadding(2);
    nested->setGap(3);
    for (int i = 0; i < 2; ++i) {
        auto* child = new Gui::Widget(nested);
        child->setFrame({0, 0, 10, 6});
    }

    auto* second = new Gui::Widget(root.get());
    auto* control = second;
    second->setFrame({0, 0, 20, 12});

    Gui::Document document(platform, std::move(root));
    document.layout({100, 20});
    EXPECT_EQ(panel->bounds(), (Rectf{0, 0, 27, 10}));
    EXPECT_EQ(control->bounds(), (Rectf{31, 0, 20, 12}));
    EXPECT_EQ(panel->children()[1]->bounds(), (Rectf{15, 2, 10, 6}));
}

TEST(GuiLayout, HiddenItemsCollapseButDisabledItemsKeepSpaceAndFractionalSizesSurvive) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Horizontal);
    root->setGap(0.5f);
    auto* first = new Gui::Widget(root.get());
    auto* a = first;
    a->setFrame({0.25f, 0.75f, 10.5f, 5.5f});
    first->setEnabled(false);

    auto* second = new Gui::Widget(root.get());
    auto* b = second;
    b->setFrame({0, 0, 20, 10});
    b->setVisible(false);

    auto* third = new Gui::Widget(root.get());
    auto* c = third;
    c->setFrame({0, 0, 10, 10});

    Gui::Document document(platform, std::move(root));
    document.layout({70000, 100});
    EXPECT_EQ(a->bounds(), (Rectf{0.25f, 0.75f, 10.5f, 5.5f}));
    EXPECT_FLOAT_EQ(c->bounds().left, 11.25f);
    EXPECT_TRUE(b->bounds().empty());
    b->setVisible(true);
    document.layout({70000, 100});
    EXPECT_FLOAT_EQ(c->bounds().left, 31.75f);
}

TEST(GuiLayout, TextIntrinsicSizeWrapReflowAndDrawingUseTheSameLines) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Horizontal);
    auto* text = new Gui::Text(root.get());
    auto* label = text;
    text->setText("中文ABCD");
    text->setFontSize(10);
    text->setWidth(Sizing::Fill);
    text->setWrap(true);

    auto* sibling = new Gui::Widget(root.get());
    sibling->setFrame({0, 0, 10, 10});

    Gui::Document document(platform, std::move(root));
    document.layout({25, 100});
    EXPECT_FLOAT_EQ(label->bounds().width, 15);
    EXPECT_FLOAT_EQ(label->bounds().height, 24);
    platform.draw(document);
    ASSERT_EQ(platform.drawnText.size(), 2);
    EXPECT_EQ(platform.drawnText[0], "中文A");
    EXPECT_EQ(platform.drawnText[1], "BCD");
    document.layout({50, 100});
    EXPECT_FLOAT_EQ(label->bounds().height, 12);
    auto natural = std::make_unique<Gui::Text>();
    natural->setFontSize(10);
    natural->setText("AB\nC");
    EXPECT_EQ(natural->measure(platform, {100, 100}), (Sizef{10, 24}));
}

TEST(GuiLayout, EmptyZeroSizeAndInvalidInput) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* child = new Gui::Widget(root.get());
    auto* control = child;
    child->setFrame({0, 0, 0, 0});

    Gui::Document document(platform, std::move(root));
    document.layout({0, 0});
    EXPECT_TRUE(control->bounds().empty());
    document.layout({10, 10});
    EXPECT_TRUE(control->bounds().empty());
    document.root().setPadding(20);
    document.layout({10, 10});
    EXPECT_TRUE(control->bounds().empty());
    Gui::Layout empty;
    empty.setPadding({1, 2, 3, 4});
    EXPECT_EQ(empty.measure(platform, {100, 100}), (Sizef{4, 6}));
    const float infinity = std::numeric_limits<float>::infinity();
    EXPECT_THROW(control->setWidth(-1), std::invalid_argument);
    EXPECT_THROW(control->setHeight(infinity), std::invalid_argument);
    EXPECT_THROW(control->setMargin({0, 0, infinity, 0}), std::invalid_argument);
    EXPECT_THROW(document.root().setPadding(-1), std::invalid_argument);
    EXPECT_THROW(document.root().setGap(1, -1), std::invalid_argument);
    EXPECT_THROW(control->measure(platform, {-1, 10}), std::invalid_argument);
    EXPECT_THROW(control->layout({0, 0, -1, 10}, platform), std::invalid_argument);
    const float maximum = std::numeric_limits<float>::max();
    EXPECT_THROW(control->layout({maximum, 0, maximum, 10}, platform), std::invalid_argument);
}
} // namespace

namespace {
TEST(GuiLayout, ColumnFillAndAutoSizeKeepMeasurementsSeparateFromBounds) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Vertical);
    root->setPadding(2);
    root->setGap(3);
    auto* header = new Gui::Widget(root.get());
    auto* top = header;
    top->setWidth(Sizing::Fill);
    top->setHeight(10);

    auto* body = new Gui::Layout(root.get());
    auto* content = body;
    body->setWidth(Sizing::Fill);
    body->setHeight(Sizing::Fill);
    body->setPadding(1);
    auto* label = new Gui::Text(body);
    auto* text = label;
    label->setText("ABCD");
    label->setFontSize(10);

    Gui::Document document(platform, std::move(root));
    document.layout({50, 60});
    EXPECT_EQ(top->bounds(), (Rectf{2, 2, 46, 10}));
    EXPECT_EQ(content->bounds(), (Rectf{2, 15, 46, 43}));
    EXPECT_EQ(text->bounds(), (Rectf{3, 16, 20, 12}));
    const auto before = text->bounds();
    const auto revision = document.revision();
    EXPECT_EQ(text->measure(platform, {100, 100}), (Sizef{20, 12}));
    EXPECT_EQ(text->bounds(), before);
    EXPECT_EQ(document.revision(), revision);
    content->setPadding(3);
    EXPECT_GT(document.revision(), revision);
    document.layout({50, 60});
    EXPECT_FLOAT_EQ(text->bounds().left, 5);
}

class InvalidMeasure final : public Gui::Widget {
public:
    using Widget::Widget;
    Sizef measure(Gui::Platform&, Sizef) const override {
        return {-1, 10};
    }
};

TEST(GuiLayout, RejectsInvalidCustomMeasurementAndRecoversAfterFailure) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* invalid = new InvalidMeasure(root.get());
    auto* bad = invalid;

    Gui::Document document(platform, std::move(root));
    EXPECT_THROW(document.layout({100, 100}), std::runtime_error);
    bad->setVisible(false);
    EXPECT_NO_THROW(document.layout({100, 100}));
    document.root().setPadding({std::numeric_limits<float>::max(), 0, 0, 0});
    EXPECT_THROW(document.root().setPadding(
                     {std::numeric_limits<float>::max(), 0, std::numeric_limits<float>::max(), 0}),
                 std::invalid_argument);
}

TEST(GuiLayout, SingleItemSpaceBetweenAndWrappedCenteringHaveFiniteBounds) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Horizontal);
    root->setJustify(Gui::Layout::Justify::SpaceBetween);
    root->setGap(5, 7);
    auto* child = new Gui::Widget(root.get());
    auto* first = child;
    child->setFrame({0, 0, 30, 10});

    Gui::Document document(platform, std::move(root));
    document.layout({100, 100});
    EXPECT_EQ(first->bounds(), (Rectf{0, 0, 30, 10}));
    auto* other = new Gui::Widget(&document.root());
    auto* second = other;
    other->setFrame({0, 0, 30, 20});

    document.root().setWrap(true);
    document.root().setAlignment(Alignment::Center);
    std::feclearexcept(FE_ALL_EXCEPT);
    document.layout({40, 100});
    EXPECT_EQ(std::fetestexcept(FE_DIVBYZERO | FE_INVALID), 0);
    EXPECT_EQ(first->bounds(), (Rectf{0, 0, 30, 10}));
    EXPECT_EQ(second->bounds(), (Rectf{0, 17, 30, 20}));
}
} // namespace

namespace {
TEST(GuiLayout, WrappedSpaceBetweenDistributesOnlyEligibleLines) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Horizontal);
    root->setWrap(true);
    root->setJustify(Gui::Layout::Justify::SpaceBetween);
    root->setGap(5);
    std::vector<Gui::Widget*> controls;
    for (int i = 0; i < 3; ++i) {
        auto* child = new Gui::Widget(root.get());
        child->setFrame({0, 0, 20, 10});
        controls.push_back(child);
    }
    Gui::Document document(platform, std::move(root));
    document.layout({60, 100});
    EXPECT_EQ(controls[0]->bounds(), (Rectf{0, 0, 20, 10}));
    EXPECT_EQ(controls[1]->bounds(), (Rectf{40, 0, 20, 10}));
    EXPECT_EQ(controls[2]->bounds(), (Rectf{0, 15, 20, 10}));
    controls[2]->setBreakBefore(true);
    document.layout({60, 100});
    EXPECT_FLOAT_EQ(controls[1]->bounds().left, 25);
}
} // namespace
