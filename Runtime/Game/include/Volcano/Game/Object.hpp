//
//
#ifndef VOLCANO_GAME_OBJECT_HPP
#define VOLCANO_GAME_OBJECT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Object: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    Object(QObject* parent = nullptr);
    Object(Context& context, QObject* parent = nullptr);
    ~Object(void) override = default;

public:
    bool isEnabled(void) const {
        return is_enabled_;
    }

    void setEnabled(bool v) {
        if (is_enabled_ != v) {
            is_enabled_ = v;
            emit enabledChanged(v);
        }
    }

    bool isVisible() const {
        return (is_enabled_ && is_visible_);
    }

    void setVisible(bool v) {
        if (is_visible_ != v) {
            is_visible_ = v;
            emit visibleChanged(v);
        }
    }

    Context* context();

    const Context* context() const {
        return context_;
    }

    void draw() const {
        if (is_enabled_ && is_visible_) {
            updateView();
        }
    }

    void tick(Duration elapsed) {
        if (is_enabled_) {
            updateState(elapsed);
        }
    }

protected:
    ResourceManager& resourceManager() {
        return context()->resourceManager();
    }

    Graphics::View& view() {
        return context()->view();
    }

    virtual void updateView() const {}
    virtual void updateState(Duration elapsed) {}

signals:
    void enabledChanged(bool v);
    void visibleChanged(bool v);

private:
    Context* context_;
    bool is_enabled_;
    bool is_visible_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_HPP
