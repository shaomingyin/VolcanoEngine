#include <algorithm>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>
#include "Platform.h"

#include <Volcano/Gui/Button.h>
#include <Volcano/Gui/Layout.h>
#include <Volcano/Gui/Document.h>
#include <Volcano/Gui/Image.h>
#include <Volcano/Gui/Text.h>

namespace {
using namespace Volcano;

TEST(GuiDocument, LayoutHitCaptureFocusAndActivation) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setLayout(Gui::Layout::Mode::Vertical);
    root->setPadding(4);
    root->setGap(3);
    auto* first = new Gui::Button(root.get());
    first->setFrame({0, 0, 50, 20});
    auto* button = first;
    int clicks = 0;
    button->setOnClick([&] {
        ++clicks;
    });

    auto* second = new Gui::Image(root.get());
    second->setFrame({0, 0, 50, 10});
    auto* image = second;

    Gui::Document document(platform, std::move(root));
    document.layout({100, 100});
    EXPECT_EQ(button->bounds(), (Rectf{4, 4, 50, 20}));
    EXPECT_EQ(image->bounds(), (Rectf{4, 27, 50, 10}));
    const auto revision = document.revision();
    EXPECT_TRUE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 8, 8}));
    EXPECT_TRUE(button->pressed());
    EXPECT_EQ(document.focus(), button);
    EXPECT_GT(document.revision(), revision);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move, 90, 90});
    EXPECT_FALSE(button->pressed());
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 90, 90});
    EXPECT_EQ(clicks, 0);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 8, 8});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 8, 8});
    EXPECT_EQ(clicks, 1);
    EXPECT_TRUE(document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Enter}));
    EXPECT_EQ(clicks, 2);
    button->setEnabled(false);
    EXPECT_FALSE(document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Space}));
    EXPECT_FALSE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 8, 8}));
    EXPECT_EQ(document.focus(), nullptr);
}

TEST(GuiDocument, RejectsInvalidGeometryAndCancelsCapture) {
    TestPlatform platform;
    EXPECT_THROW(Gui::Document(platform, nullptr), std::invalid_argument);
    auto root = std::make_unique<Gui::Layout>();
    EXPECT_THROW(root->setPadding(-1), std::invalid_argument);
    EXPECT_THROW(root->setGap(std::numeric_limits<float>::infinity()), std::invalid_argument);
    auto* button = new Gui::Button(root.get());
    auto* pointer = button;
    button->setFrame({0, 0, 20, 20});

    Gui::Document document(platform, std::move(root));
    document.layout({50, 50});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    EXPECT_TRUE(pointer->pressed());
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Cancel, 5, 5});
    EXPECT_FALSE(pointer->pressed());
    EXPECT_THROW(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move,
                                                  std::numeric_limits<float>::quiet_NaN(), 0}),
                 std::invalid_argument);
}
} // namespace

namespace {
TEST(GuiDocument, DocumentsBorrowOnePlatformAndDrawThroughIt) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    root->setBackground(Colorf::Red);
    Gui::Document first(platform, std::move(root));
    EXPECT_EQ(&first.platform(), &platform);
    first.layout({20, 20});
    platform.draw(first);
    EXPECT_EQ(platform.fills, 1);
    {
        auto other = std::make_unique<Gui::Layout>();
        other->setBackground(Colorf::Blue);
        Gui::Document second(platform, std::move(other));
        EXPECT_EQ(&second.platform(), &first.platform());
        second.layout({30, 30});
        platform.draw(second);
        EXPECT_EQ(platform.fills, 2);
    }
    platform.draw(first);
    EXPECT_EQ(platform.fills, 3);
}
} // namespace

namespace {
class InputWidget final : public Gui::Widget {
public:
    using Widget::event;
    using Widget::Widget;

    bool focusable() const noexcept override {
        return true;
    }
    bool event(const Gui::PointerEvent& input, Gui::Platform&) override {
        pointer = input;
        pointerActions.push_back(input.action);
        return true;
    }
    bool event(const Gui::ScrollEvent& input, Gui::Platform&) override {
        scroll = input;
        ++scrolls;
        return true;
    }
    bool event(const Gui::KeyEvent& input, Gui::Platform&) override {
        key = input;
        ++keys;
        return true;
    }
    bool event(const Gui::TextInputEvent& input, Gui::Platform&) override {
        text = input.text;
        return true;
    }
    bool event(const Gui::CompositionEvent& input, Gui::Platform&) override {
        composition = input;
        ++compositions;
        return true;
    }
    bool event(const Gui::FocusEvent& input, Gui::Platform&) override {
        focused = input.focused;
        ++focusChanges;
        return true;
    }

public:
    Gui::PointerEvent pointer;
    std::vector<Gui::PointerEvent::Action> pointerActions;
    Gui::ScrollEvent scroll;
    Gui::KeyEvent key;
    Gui::CompositionEvent composition;
    std::string text;
    int keys = 0;
    int scrolls = 0;
    int compositions = 0;
    int focusChanges = 0;
    bool focused = false;
};

TEST(GuiDocument, RoutesTypedInputToFocusAndScrollToHitWidget) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* first = new InputWidget(root.get());
    auto* second = new InputWidget(root.get());
    auto* a = first;
    auto* b = second;
    a->setFrame({0, 0, 20, 20});
    b->setFrame({30, 0, 20, 20});

    Gui::Document document(platform, std::move(root));
    document.layout({100, 100});
    EXPECT_FALSE(document.event(Gui::TextInputEvent{"未聚焦"}));
    EXPECT_TRUE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5}));
    EXPECT_TRUE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5}));
    EXPECT_TRUE(a->focused);
    EXPECT_EQ(a->focusChanges, 1);
    EXPECT_TRUE(document.event(Gui::TextInputEvent{"中文"}));
    EXPECT_EQ(a->text, "中文");
    Gui::KeyEvent key{Gui::KeyEvent::Action::Up, Gui::KeyEvent::Key::A};
    key.scanCode = 42;
    key.modifiers.control = true;
    EXPECT_TRUE(document.event(key));
    EXPECT_EQ(a->key.action, Gui::KeyEvent::Action::Up);
    EXPECT_EQ(a->key.scanCode, 42);
    EXPECT_TRUE(a->key.modifiers.control);
    Gui::CompositionEvent composition{Gui::CompositionEvent::Action::Update, "中文", 3, 3};
    EXPECT_TRUE(document.event(composition));
    EXPECT_EQ(a->composition.text, "中文");
    EXPECT_EQ(a->composition.selectionStart, 3);
    EXPECT_TRUE(document.event(Gui::ScrollEvent{35, 5, -2, 4, Gui::ScrollEvent::Unit::Pixels}));
    EXPECT_EQ(a->scrolls, 0);
    EXPECT_EQ(b->scrolls, 1);
    EXPECT_EQ(b->scroll.deltaY, 4);
    EXPECT_EQ(document.focus(), a);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 35, 5});
    EXPECT_FALSE(a->focused);
    EXPECT_TRUE(b->focused);
    EXPECT_EQ(a->focusChanges, 2);
    EXPECT_TRUE(document.event(Gui::FocusEvent{false}));
    EXPECT_EQ(document.focus(), nullptr);
    EXPECT_FALSE(b->focused);
    EXPECT_NE(std::ranges::find(b->pointerActions, Gui::PointerEvent::Action::Cancel),
              b->pointerActions.end());
    EXPECT_FALSE(document.event(Gui::TextInputEvent{"忽略"}));
    document.event(Gui::FocusEvent{true});
    EXPECT_FALSE(document.event(key));
}

TEST(GuiDocument, ButtonHoverSecondaryButtonAndKeyRepeatDoNotActivate) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* button = new Gui::Button(root.get());
    auto* control = button;
    control->setFrame({0, 0, 20, 20});
    int clicks = 0;
    control->setOnClick([&] {
        ++clicks;
    });

    Gui::Document document(platform, std::move(root));
    document.layout({50, 50});
    EXPECT_FALSE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move, 5, 5}));
    EXPECT_FALSE(control->pressed());
    EXPECT_FALSE(document.event(
        Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5, Gui::PointerEvent::Button::Secondary}));
    EXPECT_EQ(document.focus(), nullptr);
    EXPECT_EQ(clicks, 0);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5});
    EXPECT_EQ(clicks, 1);
    document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Repeat, Gui::KeyEvent::Key::Enter});
    document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Up, Gui::KeyEvent::Key::Enter});
    EXPECT_EQ(clicks, 1);
    document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Space});
    EXPECT_EQ(clicks, 2);
    Gui::KeyEvent shortcut{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Enter};
    shortcut.modifiers.control = true;
    EXPECT_FALSE(document.event(shortcut));
    EXPECT_EQ(clicks, 2);
}

TEST(GuiDocument, CaptureMatchesPointerAndButtonAndCancelsOnFocusLoss) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* button = new Gui::Button(root.get());
    auto* control = button;
    control->setFrame({0, 0, 20, 20});
    int clicks = 0;
    control->setOnClick([&] {
        ++clicks;
    });

    Gui::Document document(platform, std::move(root));
    document.layout({50, 50});
    Gui::PointerEvent input{Gui::PointerEvent::Action::Down, 5, 5};
    input.id = 7;
    input.device = Gui::PointerEvent::Device::Touch;
    EXPECT_TRUE(document.event(input));
    EXPECT_TRUE(control->pressed());
    input.action = Gui::PointerEvent::Action::Up;
    input.id = 8;
    EXPECT_FALSE(document.event(input));
    EXPECT_TRUE(control->pressed());
    input.id = 7;
    input.button = Gui::PointerEvent::Button::Secondary;
    EXPECT_FALSE(document.event(input));
    EXPECT_TRUE(control->pressed());
    EXPECT_TRUE(document.event(Gui::FocusEvent{false}));
    EXPECT_FALSE(control->pressed());
    document.event(Gui::FocusEvent{true});
    input.button = Gui::PointerEvent::Button::Primary;
    EXPECT_FALSE(document.event(input));
    EXPECT_EQ(clicks, 0);
    input.action = Gui::PointerEvent::Action::Down;
    document.event(input);
    control->setEnabled(false);
    input.action = Gui::PointerEvent::Action::Move;
    document.event(input);
    EXPECT_FALSE(control->pressed());
    EXPECT_EQ(clicks, 0);
}

TEST(GuiDocument, RejectsInvalidScrollAndCompositionSelection) {
    TestPlatform platform;
    Gui::Document document(platform, std::make_unique<Gui::Layout>());
    const auto infinity = std::numeric_limits<float>::infinity();
    EXPECT_THROW(document.event(Gui::ScrollEvent{0, 0, infinity, 0}), std::invalid_argument);
    EXPECT_THROW(document.event(Gui::ScrollEvent{infinity, 0, 0, 0}), std::invalid_argument);
    EXPECT_THROW(document.event(Gui::CompositionEvent{Gui::CompositionEvent::Action::Update, "中文", 1, 0}),
                 std::invalid_argument);
    EXPECT_THROW(document.event(Gui::CompositionEvent{Gui::CompositionEvent::Action::Update, "中文", 3, 1}),
                 std::invalid_argument);
    EXPECT_THROW(document.event(Gui::CompositionEvent{Gui::CompositionEvent::Action::Update, "中文", 7, 0}),
                 std::invalid_argument);
    EXPECT_THROW(document.event(Gui::CompositionEvent{Gui::CompositionEvent::Action::Update, "中文", 3,
                                                      std::numeric_limits<std::size_t>::max()}),
                 std::invalid_argument);
    EXPECT_FALSE(document.event(Gui::CompositionEvent{Gui::CompositionEvent::Action::Cancel}));
}

TEST(GuiDocument, UnhandledEventsReturnFalseAndHiddenAncestorInvalidatesFocus) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* button = new Gui::Button(root.get());
    button->setFrame({0, 0, 20, 20});

    Gui::Document document(platform, std::move(root));
    document.layout({50, 50});
    EXPECT_FALSE(document.event(Gui::ScrollEvent{40, 40, 0, 1}));
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5});
    document.root().setVisible(false);
    EXPECT_FALSE(document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Enter}));
    EXPECT_EQ(document.focus(), nullptr);
}
} // namespace

namespace {
TEST(GuiDocument, GeneratesHoverTransitionsAndDeliversCapturedMovesOutside) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* widget = new InputWidget(root.get());
    auto* target = widget;
    widget->setFrame({0, 0, 20, 20});

    Gui::Document document(platform, std::move(root));
    document.layout({50, 50});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move, 5, 5});
    ASSERT_EQ(target->pointerActions.size(), 2);
    EXPECT_EQ(target->pointerActions.front(), Gui::PointerEvent::Action::Enter);
    EXPECT_EQ(target->pointerActions.back(), Gui::PointerEvent::Action::Move);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move, 30, 30});
    EXPECT_EQ(target->pointerActions.back(), Gui::PointerEvent::Action::Leave);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move, 80, 80});
    EXPECT_EQ(target->pointer.action, Gui::PointerEvent::Action::Move);
    EXPECT_EQ(target->pointer.x, 80);
    EXPECT_FALSE(target->pointer.inside);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 80, 80});
    EXPECT_EQ(target->pointer.action, Gui::PointerEvent::Action::Up);
    EXPECT_FALSE(target->pointer.inside);
}
} // namespace

namespace {
TEST(GuiDocument, KeyboardFocusIsIndependentOfOcclusionAndChecksAncestors) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* container = new Gui::Layout(root.get());
    auto* parent = container;
    parent->setFrame({0, 0, 40, 40});
    auto* input = new InputWidget(container);
    auto* target = input;
    target->setFrame({0, 0, 40, 40});

    auto* overlay = new Gui::Image(root.get());
    overlay->setFrame({10, 10, 20, 20});

    Gui::Document document(platform, std::move(root));
    document.layout({50, 50});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5});
    EXPECT_EQ(document.focus(), target);
    EXPECT_TRUE(document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::A}));
    parent->setEnabled(false);
    EXPECT_FALSE(document.event(Gui::TextInputEvent{"ignored"}));
    EXPECT_EQ(document.focus(), nullptr);
    EXPECT_FALSE(target->focused);
}
} // namespace

namespace {
TEST(GuiStyle, PlatformStyleIsUsedAtDrawTimeAndExplicitTextColorTakesPriority) {
    TestPlatform platform;
    const Gui::Platform& services = platform;
    EXPECT_EQ(&services.style(), &platform.currentStyle);
    EXPECT_EQ(services.style().textColor(), Colorf::White);
    EXPECT_EQ(services.style().buttonPressedColor(), (Colorf{0, 0, 0, 0.2f}));
    auto root = std::make_unique<Gui::Layout>();
    auto* text = new Gui::Text(root.get());
    text->setFrame({0, 0, 40, 20});
    text->setText("text");
    Gui::Document document(platform, std::move(root));
    document.layout({100, 100});
    platform.draw(document);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::White}));

    platform.currentStyle.setTextColor(Colorf::Red);
    platform.colors.clear();
    platform.draw(document);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::Red}));

    text->setColor(Colorf::Blue);
    platform.currentStyle.setTextColor(Colorf::Green);
    platform.colors.clear();
    platform.draw(document);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::Blue}));
}

TEST(GuiStyle, ButtonUsesCurrentPressedOverlayAndDocumentsShareStyle) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* button = new Gui::Button(root.get());
    button->setFrame({0, 0, 40, 20});
    Gui::Document first(platform, std::move(root));
    auto otherRoot = std::make_unique<Gui::Layout>();
    auto* other = new Gui::Button(otherRoot.get());
    other->setFrame({0, 0, 40, 20});
    Gui::Document second(platform, std::move(otherRoot));
    first.layout({100, 100});
    second.layout({100, 100});
    platform.draw(first);
    EXPECT_TRUE(platform.colors.empty());
    button->setPressed(true);
    platform.draw(first);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf{0, 0, 0, 0.2f}}));
    platform.currentStyle.setButtonPressedColor(Colorf::Red);
    other->setPressed(true);
    platform.colors.clear();
    platform.draw(first);
    platform.draw(second);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::Red, Colorf::Red}));
}
} // namespace

namespace {
TEST(GuiStyle, FontSizeAndLineHeightDriveMeasurementWrappingAndDrawing) {
    TestPlatform platform;
    platform.currentStyle.setFontSize(10);
    platform.currentStyle.setLineHeight(2);
    auto root = std::make_unique<Gui::Layout>();
    auto* text = new Gui::Text(root.get());
    text->setWidth(20);
    text->setText("abcdef");
    text->setWrap(true);
    Gui::Document document(platform, std::move(root));
    document.layout({100, 200});
    EXPECT_FLOAT_EQ(text->fontSize(platform), 10);
    EXPECT_FLOAT_EQ(text->bounds().height, 40);
    platform.draw(document);
    EXPECT_EQ(platform.drawnText, (std::vector<std::string>{"abcd", "ef"}));
    EXPECT_EQ(platform.textSizes, (std::vector<float>{10, 10}));
    EXPECT_EQ(platform.textBaselines, (std::vector<float>{10, 30}));

    platform.currentStyle.setFontSize(16);
    document.layout({100, 200});
    EXPECT_FLOAT_EQ(text->bounds().height, 96);
    text->setFontSize(20);
    platform.currentStyle.setFontSize(12);
    document.layout({100, 200});
    EXPECT_FLOAT_EQ(text->fontSize(platform), 20);
    EXPECT_FLOAT_EQ(text->bounds().height, 120);
    platform.drawnText.clear();
    platform.textSizes.clear();
    platform.textBaselines.clear();
    platform.draw(document);
    EXPECT_EQ(platform.drawnText, (std::vector<std::string>{"ab", "cd", "ef"}));
    EXPECT_EQ(platform.textSizes, (std::vector<float>{20, 20, 20}));
    EXPECT_EQ(platform.textBaselines, (std::vector<float>{20, 60, 100}));
}

TEST(GuiStyle, DisabledTextUsesAncestorStateAndExplicitColorStillWins) {
    TestPlatform platform;
    platform.currentStyle.setDisabledTextColor(Colorf::Green);
    Gui::Widget parent;
    Gui::Text text(&parent);
    text.setFrame({0, 0, 40, 20});
    text.setText("text");
    parent.layout({0, 0, 100, 100}, platform);
    parent.setEnabled(false);
    platform.draw(parent);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::Green}));
    text.setColor(Colorf::Blue);
    platform.colors.clear();
    platform.draw(parent);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::Blue}));
}

TEST(GuiStyle, BackgroundAndInsetBorderUseStyleAndExplicitTransparentBackgroundWins) {
    TestPlatform platform;
    platform.currentStyle.setBackgroundColor(Colorf::Red);
    platform.currentStyle.setBorderColor(Colorf::Blue);
    platform.currentStyle.setBorderWidth(4);
    Gui::Widget widget;
    widget.layout({10, 20, 40, 30}, platform);
    EXPECT_EQ(widget.background(platform), Colorf::Red);
    platform.draw(widget);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::Red, Colorf::Blue}));
    EXPECT_EQ(platform.rectangles, (std::vector<Rectf>{{10, 20, 40, 30}, {12, 22, 36, 26}}));
    EXPECT_EQ(platform.strokes, (std::vector<float>{4}));
    widget.setBackground(Colorf::Transparent);
    EXPECT_EQ(widget.background(platform), Colorf::Transparent);
    platform.colors.clear();
    platform.draw(widget);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::Blue}));
    platform.currentStyle.setBorderWidth(0);
    platform.colors.clear();
    platform.draw(widget);
    EXPECT_TRUE(platform.colors.empty());

    platform.currentStyle.setBorderWidth(20);
    widget.layout({0, 0, 2, 1}, platform);
    platform.rectangles.clear();
    platform.strokes.clear();
    platform.draw(widget);
    EXPECT_EQ(platform.strokes, (std::vector<float>{1}));
    EXPECT_EQ(platform.rectangles, (std::vector<Rectf>{{0.5f, 0.5f, 1, 0}}));
    widget.layout({0, 0, 0, 0}, platform);
    platform.strokes.clear();
    platform.draw(widget);
    EXPECT_TRUE(platform.strokes.empty());
}

TEST(GuiStyle, ImageUsesCurrentTint) {
    TestPlatform platform;
    Gui::Image image;
    image.setSource("image");
    image.layout({0, 0, 40, 20}, platform);
    platform.draw(image);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::White}));
    platform.currentStyle.setImageColor(Colorf::Red);
    platform.colors.clear();
    platform.draw(image);
    EXPECT_EQ(platform.colors, (std::vector<Colorf>{Colorf::Red}));
}

TEST(GuiStyle, RejectsInvalidMetricsWithoutChangingConfiguration) {
    Gui::Style style;
    for (const float value : {-1.0f, 0.0f, 4097.0f, std::numeric_limits<float>::infinity(),
                              std::numeric_limits<float>::quiet_NaN()}) {
        EXPECT_THROW(style.setFontSize(value), std::invalid_argument);
    }
    for (const float value :
         {-1.0f, 0.0f, std::numeric_limits<float>::max(), std::numeric_limits<float>::infinity(),
          std::numeric_limits<float>::quiet_NaN()}) {
        EXPECT_THROW(style.setLineHeight(value), std::invalid_argument);
    }
    for (const float value :
         {-1.0f, std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()}) {
        EXPECT_THROW(style.setBorderWidth(value), std::invalid_argument);
    }
    EXPECT_FLOAT_EQ(style.fontSize(), 16);
    EXPECT_FLOAT_EQ(style.lineHeight(), 1.2f);
    EXPECT_FLOAT_EQ(style.borderWidth(), 0);
    EXPECT_NO_THROW(style.setFontSize(1));
    EXPECT_NO_THROW(style.setFontSize(4096));
    EXPECT_NO_THROW(style.setLineHeight(0.5f));
    EXPECT_NO_THROW(style.setBorderWidth(0));
}

TEST(GuiDocument, ImplementsTextureInterfaceAndValidatesSize) {
    Gui::Document doc(std::make_unique<Gui::Layout>(), Size{128, 64});
    EXPECT_EQ(doc.size(), (Size{128, 64}));

    doc.setSize({256, 128});
    EXPECT_EQ(doc.size(), (Size{256, 128}));

    const World::Texture& asTexture = doc;
    EXPECT_EQ(asTexture.size(), (Size{256, 128}));

    const World::Canvas& asCanvas = doc;
    EXPECT_EQ(asCanvas.size(), (Size{256, 128}));

    EXPECT_THROW(doc.setSize({-1, 64}), std::invalid_argument);
    EXPECT_THROW(doc.setSize({128, -1}), std::invalid_argument);
}

TEST(GuiDocument, RecordsCommandsDirectlyIntoCanvas) {
    auto root = std::make_unique<Gui::Layout>();
    root->setBackground(Colorf::Red);
    Gui::Document doc(std::move(root), Size{100, 100});
    doc.layout({100, 100});
    doc.draw();

    EXPECT_EQ(doc.commands().size(), 1);
}
} // namespace
