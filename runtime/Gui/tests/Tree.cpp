#include <algorithm>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "Platform.h"

#include <Volcano/Gui/Button.h>
#include <Volcano/Gui/Layout.h>
#include <Volcano/Gui/Document.h>
#include <Volcano/Gui/Image.h>
#include <Volcano/Gui/Text.h>
#include <Volcano/Gui/Widget.h>

namespace {
using namespace Volcano;

struct Interaction {
    int destroyed = 0;
    int cancelled = 0;
    int left = 0;
    int lostFocus = 0;
    bool fail = false;
    Gui::PointerEvent lastLeave;
};

class TrackedWidget final : public Gui::Widget {
public:
    explicit TrackedWidget(Interaction& interaction, Widget* parent = nullptr)
        : Widget(parent), interaction_(interaction) {}
    ~TrackedWidget() override {
        ++interaction_.destroyed;
    }
    using Widget::event;
    bool focusable() const noexcept override {
        return true;
    }
    bool event(const Gui::PointerEvent& input, Gui::Platform&) override {
        if (input.action == Gui::PointerEvent::Action::Cancel) {
            ++interaction_.cancelled;
        }
        if (input.action == Gui::PointerEvent::Action::Leave) {
            ++interaction_.left;
            interaction_.lastLeave = input;
        }
        return true;
    }
    bool event(const Gui::FocusEvent& input, Gui::Platform&) override {
        if (!input.focused) {
            ++interaction_.lostFocus;
            if (interaction_.fail) {
                throw std::runtime_error("Focus handler failed");
            }
        }
        return true;
    }

private:
    Interaction& interaction_;
};

TEST(GuiTree, ConstructorParentAndReparentTransferOwnership) {
    Gui::Layout first;
    Gui::Image second;
    auto* parent = new Gui::Button(&first);
    auto* child = new Gui::Text(parent);
    EXPECT_EQ(parent->parent(), &first);
    EXPECT_EQ(child->parent(), parent);
    EXPECT_EQ(&child->root(), &first);
    EXPECT_TRUE(first.contains(*child));
    const Gui::Widget& constChild = *child;
    EXPECT_EQ(constChild.parent(), parent);
    EXPECT_EQ(&constChild.root(), &first);

    const auto oldRevision = first.revision();
    const auto newRevision = second.revision();
    parent->setParent(&second);
    EXPECT_TRUE(first.children().empty());
    EXPECT_EQ(parent->parent(), &second);
    EXPECT_EQ(child->parent(), parent);
    EXPECT_EQ(&child->root(), &second);
    EXPECT_GT(first.revision(), oldRevision);
    EXPECT_GT(second.revision(), newRevision);
    const auto unchanged = second.revision();
    parent->setParent(&second);
    EXPECT_EQ(second.revision(), unchanged);
    EXPECT_EQ(second.children().size(), 1);

    parent->setParent(nullptr);
    std::unique_ptr<Gui::Widget> detached(parent);
    EXPECT_TRUE(second.children().empty());
    EXPECT_EQ(parent->parent(), nullptr);
    EXPECT_EQ(&child->root(), parent);
    child->setBackground(Colorf::Blue);
    EXPECT_FALSE(second.contains(*child));
}

TEST(GuiTree, StackChildUnlinksBeforeParentDestruction) {
    Gui::Widget parent;
    {
        Gui::Text child(&parent);
        EXPECT_EQ(parent.children().front(), &child);
    }
    EXPECT_TRUE(parent.children().empty());
}

TEST(GuiTree, ParentDeletesChildrenAndExplicitDeleteUnlinks) {
    Interaction first;
    Interaction second;
    {
        Gui::Widget parent;
        auto* child = new TrackedWidget(first, &parent);
        new TrackedWidget(second, child);
        delete child;
        EXPECT_TRUE(parent.children().empty());
        EXPECT_EQ(first.destroyed, 1);
        EXPECT_EQ(second.destroyed, 1);
        new TrackedWidget(first, &parent);
    }
    EXPECT_EQ(first.destroyed, 2);
}

TEST(GuiTree, FailedConstructionUnlinksFromParent) {
    class FailingWidget final : public Gui::Widget {
    public:
        explicit FailingWidget(Widget* parent) : Widget(parent) {
            throw std::runtime_error("Construction failed");
        }
    };
    Gui::Widget parent;
    EXPECT_THROW(new FailingWidget(&parent), std::runtime_error);
    EXPECT_TRUE(parent.children().empty());
}

TEST(GuiTree, RevisionsAndInputEligibilityFollowParentChain) {
    Gui::Widget root;
    auto* parent = new Gui::Widget(&root);
    auto* child = new Gui::Widget(parent);
    const auto revision = root.revision();
    child->setBackground(Colorf::Red);
    EXPECT_GT(root.revision(), revision);
    EXPECT_TRUE(root.acceptsInput(*child));
    parent->setEnabled(false);
    EXPECT_FALSE(root.acceptsInput(*child));
    parent->setEnabled(true);
    parent->setVisible(false);
    EXPECT_FALSE(root.acceptsInput(*child));
    parent->setVisible(true);
    EXPECT_TRUE(root.acceptsInput(*child));
}

TEST(GuiTree, DetachingSubtreeClearsDocumentInteractionBeforeDestruction) {
    TestPlatform platform;
    Interaction interaction;
    auto root = std::make_unique<Gui::Layout>();
    auto* parent = new Gui::Widget(root.get());
    parent->setFrame({0, 0, 40, 40});
    auto* child = new TrackedWidget(interaction, parent);
    child->setFrame({0, 0, 20, 20});
    Gui::Document document(platform, std::move(root));
    document.layout({100, 100});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    EXPECT_EQ(document.focus(), child);
    parent->setParent(nullptr);
    std::unique_ptr<Gui::Widget> detached(parent);
    EXPECT_EQ(document.focus(), nullptr);
    EXPECT_EQ(child->parent(), parent);
    EXPECT_EQ(interaction.cancelled, 1);
    EXPECT_EQ(interaction.left, 1);
    EXPECT_EQ(interaction.lostFocus, 1);
    EXPECT_EQ(interaction.destroyed, 0);
    detached.reset();
    EXPECT_EQ(interaction.destroyed, 1);
    EXPECT_FALSE(document.event(Gui::KeyEvent{Gui::KeyEvent::Action::Down, Gui::KeyEvent::Key::Enter}));
    EXPECT_FALSE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5}));
}

TEST(GuiTree, DeletingSubtreeClearsOnlyItsDocumentReferencesWithoutCallbacks) {
    TestPlatform platform;
    Interaction first;
    Interaction second;
    auto root = std::make_unique<Gui::Layout>();
    auto* parent = new Gui::Widget(root.get());
    parent->setFrame({0, 0, 40, 40});
    auto* child = new TrackedWidget(first, parent);
    child->setFrame({0, 0, 20, 20});
    auto* unrelated = new TrackedWidget(second, root.get());
    unrelated->setFrame({50, 0, 20, 20});
    Gui::Document document(platform, std::move(root));
    document.layout({100, 100});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    delete unrelated;
    EXPECT_EQ(document.focus(), child);
    first.fail = true;
    delete parent;
    EXPECT_EQ(document.focus(), nullptr);
    EXPECT_EQ(first.destroyed, 1);
    EXPECT_EQ(first.cancelled, 0);
    EXPECT_EQ(first.lostFocus, 0);
    EXPECT_TRUE(document.root().children().empty());
    EXPECT_FALSE(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5}));
}

TEST(GuiTree, FailedReparentNotificationPreservesOriginalOwnership) {
    TestPlatform platform;
    Interaction interaction;
    Gui::Widget other;
    auto root = std::make_unique<Gui::Layout>();
    auto* child = new TrackedWidget(interaction, root.get());
    child->setFrame({0, 0, 20, 20});
    Gui::Document document(platform, std::move(root));
    document.layout({100, 100});
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    interaction.fail = true;
    EXPECT_THROW(child->setParent(&other), std::runtime_error);
    EXPECT_EQ(document.focus(), nullptr);
    EXPECT_EQ(child->parent(), &document.root());
    EXPECT_EQ(document.root().children().front(), child);
    EXPECT_TRUE(other.children().empty());
    interaction.fail = false;
    child->setParent(&other);
    EXPECT_EQ(child->parent(), &other);
    EXPECT_TRUE(document.root().children().empty());
}

TEST(GuiTree, TransferBetweenDocumentsCancelsOldInteraction) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* child = new Gui::Button(root.get());
    child->setFrame({0, 0, 20, 20});
    int clicks = 0;
    child->setOnClick([&] {
        ++clicks;
    });
    Gui::Document a(platform, std::move(root));
    Gui::Document b(platform, std::make_unique<Gui::Layout>());
    a.layout({50, 50});
    a.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    EXPECT_TRUE(child->pressed());
    child->setParent(&b.root());
    EXPECT_EQ(a.focus(), nullptr);
    EXPECT_FALSE(child->pressed());
    b.layout({50, 50});
    b.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    b.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5});
    EXPECT_EQ(clicks, 1);
}

TEST(GuiTree, DetachingHoveredWidgetPreservesPointerDetails) {
    TestPlatform platform;
    Interaction interaction;
    auto root = std::make_unique<Gui::Layout>();
    auto* child = new TrackedWidget(interaction, root.get());
    child->setFrame({0, 0, 20, 20});
    Gui::Document document(platform, std::move(root));
    document.layout({50, 50});
    Gui::PointerEvent input{Gui::PointerEvent::Action::Move, 7, 9};
    input.id = 42;
    input.device = Gui::PointerEvent::Device::Pen;
    document.event(input);
    child->setParent(nullptr);
    std::unique_ptr<Gui::Widget> detached(child);
    EXPECT_EQ(interaction.left, 1);
    EXPECT_EQ(interaction.cancelled, 0);
    EXPECT_EQ(interaction.lastLeave.id, 42);
    EXPECT_EQ(interaction.lastLeave.device, Gui::PointerEvent::Device::Pen);
    EXPECT_FLOAT_EQ(interaction.lastLeave.x, 7);
    EXPECT_FLOAT_EQ(interaction.lastLeave.y, 9);
}

TEST(GuiTree, DocumentDestructionDoesNotDispatchUserEvents) {
    TestPlatform platform;
    Interaction interaction;
    {
        auto root = std::make_unique<Gui::Layout>();
        auto* child = new TrackedWidget(interaction, root.get());
        child->setFrame({0, 0, 20, 20});
        Gui::Document document(platform, std::move(root));
        document.layout({50, 50});
        document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
        interaction.fail = true;
    }
    EXPECT_EQ(interaction.destroyed, 1);
    EXPECT_EQ(interaction.lostFocus, 0);
}

TEST(GuiTree, CompositeWidgetsArrangeDrawHitAndCollectResources) {
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* parent = new Gui::Button(root.get());
    parent->setFrame({10, 20, 60, 40});
    parent->setBackground(Colorf::Red);
    parent->setText("parent");
    parent->setFont("parent.ttf");
    auto* child = new Gui::Text(parent);
    child->setFrame({3, 4, 20, 10});
    child->setBackground(Colorf::Blue);
    child->setText("child");
    child->setFont("child.ttf");
    auto* image = new Gui::Image(child);
    image->setSource("picture");
    Gui::Document document(platform, std::move(root));
    document.layout({100, 100});
    EXPECT_EQ(child->bounds(), (Rectf{13, 24, 20, 10}));
    EXPECT_EQ(document.root().hit(15, 25), child);
    EXPECT_EQ(document.root().hit(50, 50), parent);
    document.draw();
    EXPECT_EQ(platform.fills, 2);
    EXPECT_EQ(platform.drawnText, (std::vector<std::string>{"parent", "child"}));
    auto* overlay = new Gui::Widget(parent);
    overlay->setFrame({3, 4, 20, 10});
    document.layout({100, 100});
    EXPECT_EQ(document.root().hit(15, 25), overlay);
    overlay->setVisible(false);
    document.layout({100, 100});
    EXPECT_EQ(document.root().hit(15, 25), child);
    child->setFrame({55, 0, 20, 20});
    document.layout({100, 100});
    EXPECT_NE(document.root().hit(75, 25), child);
    std::vector<std::filesystem::path> fonts;
    std::vector<std::string> images;
    document.root().collectResources(fonts, images);
    EXPECT_EQ(fonts.size(), 2);
    EXPECT_NE(std::find(fonts.begin(), fonts.end(), "parent.ttf"), fonts.end());
    EXPECT_NE(std::find(fonts.begin(), fonts.end(), "child.ttf"), fonts.end());
    EXPECT_EQ(images, (std::vector<std::string>{"picture"}));
}

#ifdef VOLCANO_DEBUG
TEST(GuiTree, RejectsCyclesAndTreeChangesDuringCallbacks) {
    EXPECT_DEATH(
        {
            Gui::Widget widget;
            widget.setParent(&widget);
        },
        ".*");
    EXPECT_DEATH(
        {
            Gui::Widget root;
            auto* child = new Gui::Widget(&root);
            root.setParent(child);
        },
        ".*");
    TestPlatform platform;
    auto root = std::make_unique<Gui::Layout>();
    auto* child = new Gui::Button(root.get());
    child->setFrame({0, 0, 20, 20});
    Gui::Document document(platform, std::move(root));
    document.layout({50, 50});
    child->setOnClick([&] {
        child->setParent(nullptr);
    });
    document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Down, 5, 5});
    EXPECT_DEATH(document.event(Gui::PointerEvent{Gui::PointerEvent::Action::Up, 5, 5}), ".*");
}
#endif
} // namespace
