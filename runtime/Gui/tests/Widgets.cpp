#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>
#include "Platform.h"

#include <Volcano/Gui/CheckBox.h>
#include <Volcano/Gui/Document.h>
#include <Volcano/Gui/Layout.h>
#include <Volcano/Gui/ProgressBar.h>
#include <Volcano/Gui/Slider.h>

namespace {
using namespace Volcano;

TEST(GuiWidgets, CheckBoxClicksKeyboardAndCancellation) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* box = new Gui::CheckBox(root.get());
    box->setFrame({0, 0, 20, 20});
    std::vector<bool> toggled;
    box->setOnToggled([&](bool value) {
        toggled.push_back(value);
    });
    Gui::Document document(platform, std::move(root));
    document.layout({100, 100});
    Gui::PointerEvent down{Gui::PointerEvent::Action::Down, 5, 5};
    down.button = Gui::PointerEvent::Button::Secondary;
    EXPECT_FALSE(document.event(down));
    EXPECT_FALSE(box->checked());
    down.button = Gui::PointerEvent::Button::Primary;
    EXPECT_TRUE(document.event(down));
    EXPECT_EQ(document.focus(), box);
    EXPECT_TRUE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5}));
    EXPECT_TRUE(box->checked());
    EXPECT_EQ(toggled, (std::vector<bool>{true}));
    document.event(down);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 50, 50});
    EXPECT_TRUE(box->checked());
    document.event(down);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Cancel, 5, 5});
    EXPECT_FALSE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5}));
    EXPECT_TRUE(box->checked());
    EXPECT_TRUE(document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Space}));
    EXPECT_FALSE(box->checked());
    document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Repeat, Gui::KeyEvent::Key::Space});
    document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Up, Gui::KeyEvent::Key::Space});
    EXPECT_FALSE(box->checked());
    EXPECT_EQ(toggled, (std::vector<bool>{true, false}));
    document.event(down);
    document.event(Gui::FocusEvent{false});
    document.event(Gui::FocusEvent{true});
    EXPECT_FALSE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5}));
    box->setEnabled(false);
    EXPECT_FALSE(document.event(down));
    EXPECT_FALSE(box->checked());
}

TEST(GuiWidgets, CheckBoxSettersNotifyOnlyOnChangeAndCallbackMayReplaceItself) {
    Gui::CheckBox box;
    std::vector<bool> toggled;
    box.setOnToggled([&](bool value) {
        toggled.push_back(value);
        box.setOnToggled({});
    });
    const auto revision = box.revision();
    box.setChecked(false);
    EXPECT_EQ(box.revision(), revision);
    box.setChecked(true);
    EXPECT_GT(box.revision(), revision);
    box.setChecked(true);
    box.setChecked(false);
    EXPECT_EQ(toggled, (std::vector<bool>{true}));
    box.setOnToggled([](bool) {
        throw std::runtime_error("callback failed");
    });
    EXPECT_THROW(box.setChecked(true), std::runtime_error);
    EXPECT_TRUE(box.checked());
}

TEST(GuiWidgets, SliderCapturesDragOutsideBoundsAndCancelsOnFocusLoss) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* slider = new Gui::Slider(root.get());
    slider->setFrame({0, 0, 100, 20});
    std::vector<float> values;
    slider->setOnValueChanged([&](float value) {
        values.push_back(value);
    });
    Gui::Document document(platform, std::move(root));
    document.layout({200, 100});
    EXPECT_TRUE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 50, 10}));
    EXPECT_FLOAT_EQ(slider->value(), 0.5f);
    EXPECT_EQ(document.focus(), slider);
    EXPECT_TRUE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move, 200, 50}));
    EXPECT_FLOAT_EQ(slider->value(), 1);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move, -20, 50});
    EXPECT_FLOAT_EQ(slider->value(), 0);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 50, 10});
    EXPECT_FLOAT_EQ(slider->value(), 0.5f);
    EXPECT_EQ(values, (std::vector<float>{0.5f, 1, 0, 0.5f}));
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 50, 10});
    document.event(Gui::FocusEvent{false});
    document.event(Gui::FocusEvent{true});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move, 90, 10});
    EXPECT_FLOAT_EQ(slider->value(), 0.5f);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 50, 10});
    slider->setEnabled(false);
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Move, 90, 10});
    EXPECT_FLOAT_EQ(slider->value(), 0.5f);
}

TEST(GuiWidgets, SliderKeyboardUsesStepAndLimitsAndRejectsModifiers) {
    TestPlatform platform;
    Gui::Slider slider;
    slider.setRange(-10, 10);
    slider.setStep(3);
    EXPECT_TRUE(
        slider.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Right}, platform));
    EXPECT_FLOAT_EQ(slider.value(), 3);
    slider.event(Gui::KeyEvent{Gui::KeyEvent::Action::Repeat, Gui::KeyEvent::Key::Up}, platform);
    EXPECT_FLOAT_EQ(slider.value(), 6);
    slider.event(Gui::KeyEvent{Gui::KeyEvent::Action::Up, Gui::KeyEvent::Key::Up}, platform);
    EXPECT_FLOAT_EQ(slider.value(), 6);
    slider.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::End}, platform);
    EXPECT_FLOAT_EQ(slider.value(), 10);
    slider.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Right}, platform);
    EXPECT_FLOAT_EQ(slider.value(), 10);
    slider.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Home}, platform);
    EXPECT_FLOAT_EQ(slider.value(), -10);
    Gui::KeyEvent modified{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Right};
    modified.modifiers.control = true;
    EXPECT_FALSE(slider.event(modified, platform));
    slider.setEnabled(false);
    modified.modifiers.control = false;
    EXPECT_FALSE(slider.event(modified, platform));
    EXPECT_FLOAT_EQ(slider.value(), -10);
}

TEST(GuiWidgets, SliderRangeClampsAndHandlesExtremeFiniteValues) {
    TestPlatform platform;
    Gui::Slider slider;
    std::vector<float> values;
    slider.setOnValueChanged([&](float value) {
        values.push_back(value);
    });
    slider.setValue(0.5f);
    slider.setValue(0.5f);
    slider.setValue(2);
    slider.setRange(10, 20);
    EXPECT_EQ(values, (std::vector<float>{0.5f, 1, 10}));
    const auto revision = slider.revision();
    EXPECT_THROW(slider.setRange(20, 10), std::invalid_argument);
    EXPECT_THROW(slider.setRange(10, 10), std::invalid_argument);
    EXPECT_EQ(slider.revision(), revision);
    for (float value : {std::numeric_limits<float>::infinity(), std::numeric_limits<float>::quiet_NaN()}) {
        EXPECT_THROW(slider.setRange(value, 20), std::invalid_argument);
        EXPECT_THROW(slider.setRange(10, value), std::invalid_argument);
        EXPECT_THROW(slider.setValue(value), std::invalid_argument);
        EXPECT_THROW(slider.setStep(value), std::invalid_argument);
    }
    EXPECT_THROW(slider.setStep(0), std::invalid_argument);
    EXPECT_THROW(slider.setStep(-1), std::invalid_argument);
    slider.setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    slider.layout({0, 0, 100, 20}, platform);
    slider.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 50, 10}, platform);
    EXPECT_FLOAT_EQ(slider.value(), 0);
    slider.setValue(std::numeric_limits<float>::max());
    slider.setStep(std::numeric_limits<float>::max());
    EXPECT_NO_THROW(
        slider.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Right}, platform));
    EXPECT_FLOAT_EQ(slider.value(), std::numeric_limits<float>::max());
    platform.rectangles.clear();
    slider.draw(platform);
    EXPECT_EQ(platform.rectangles.back(), (Rectf{80, 0, 20, 20}));
}

TEST(GuiWidgets, ProgressClampsAndDrawsFractionWithoutInputHandling) {
    TestPlatform platform;
    Gui::ProgressBar progress;
    progress.layout({0, 0, 100, 20}, platform);
    progress.draw(platform);
    EXPECT_EQ(platform.fills, 0);
    EXPECT_EQ(platform.strokes.size(), 1);
    const auto revision = progress.revision();
    progress.setValue(0);
    EXPECT_EQ(progress.revision(), revision);
    progress.setValue(0.5f);
    EXPECT_GT(progress.revision(), revision);
    platform.rectangles.clear();
    progress.draw(platform);
    EXPECT_EQ(platform.rectangles.back(), (Rectf{1, 1, 49, 18}));
    progress.setValue(-1);
    EXPECT_FLOAT_EQ(progress.value(), 0);
    progress.setValue(2);
    EXPECT_FLOAT_EQ(progress.value(), 1);
    EXPECT_THROW(progress.setValue(std::numeric_limits<float>::infinity()), std::invalid_argument);
    EXPECT_THROW(progress.setValue(std::numeric_limits<float>::quiet_NaN()), std::invalid_argument);
    EXPECT_FLOAT_EQ(progress.value(), 1);
    EXPECT_FALSE(progress.focusable());
    EXPECT_FALSE(progress.event(Gui::PointerEvent{}, platform));
}

TEST(GuiWidgets, MeasurementsFollowStyleAndDrawingHandlesZeroSize) {
    TestPlatform platform;
    platform.currentStyle.setFontSize(20);
    Gui::CheckBox box;
    Gui::Slider slider;
    Gui::ProgressBar progress;
    EXPECT_EQ(box.measure(platform, {200, 100}), (Sizef{20, 20}));
    EXPECT_EQ(slider.measure(platform, {200, 100}), (Sizef{160, 20}));
    EXPECT_EQ(progress.measure(platform, {200, 100}), (Sizef{160, 20}));
    box.setWidth(Gui::Widget::Sizing::Fill);
    EXPECT_EQ(box.measure(platform, {200, 100}), (Sizef{200, 20}));
    slider.setFrame({0, 0, 50, 10});
    EXPECT_EQ(slider.measure(platform, {200, 100}), (Sizef{50, 10}));
    box.layout({0, 0, 0, 0}, platform);
    slider.layout({0, 0, 0, 0}, platform);
    progress.layout({0, 0, 0, 0}, platform);
    box.draw(platform);
    slider.draw(platform);
    progress.draw(platform);
    EXPECT_TRUE(platform.rectangles.empty());
    EXPECT_FALSE(slider.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down}, platform));
    slider.layout({0, 0, 10, 20}, platform);
    EXPECT_FALSE(slider.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5}, platform));
}

TEST(GuiWidgets, CheckBoxDrawsCheckAndDisabledState) {
    TestPlatform platform;
    Gui::CheckBox box;
    box.layout({0, 0, 20, 20}, platform);
    box.draw(platform);
    EXPECT_EQ(platform.strokes.size(), 1);
    EXPECT_EQ(platform.colors.back(), platform.style().textColor());
    platform.strokes.clear();
    box.setChecked(true);
    box.setEnabled(false);
    box.draw(platform);
    EXPECT_EQ(platform.strokes.size(), 2);
    EXPECT_EQ(platform.colors.back(), platform.style().disabledTextColor());
}
} // namespace
