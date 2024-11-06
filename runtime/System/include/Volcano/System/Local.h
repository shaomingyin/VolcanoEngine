//
//
#ifndef VOLCANO_SYSTEM_LOCAL_H
#define VOLCANO_SYSTEM_LOCAL_H

#include <atomic>
#include <memory>

#include <Volcano/Error.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/Graphics/BulletDebugDrawer.h>
#include <Volcano/Sound/Space.h>
#include <Volcano/Gui/Window.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/Window.h>
#include <Volcano/System/Input.h>
#include <Volcano/System/Console.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class Local: public Base {
public:
	Local();
	virtual ~Local();

public:
    bool isPhysicsDebugDrawEnabled() const {
        return (physicsDebugDrawer() != nullptr);
    }

    void enablePhysicsDebugDraw() {
        setPhysicsDebugDrawer(&bullet_debug_drawer_);
    }

    void disablePhysicsDebugDraw() {
        setPhysicsDebugDrawer(nullptr);
    }

protected:
	void frame(Duration elapsed) override;
    void loadingFrame(Duration elapsed) override;

protected:
    virtual void onWindowMoved(int x, int y) {}
    virtual void onWindowResized(int w, int h) {}
    virtual void onWindowShown() {}
    virtual void onWindowHidden() {}
    virtual void onWindowFocusIn(bool focused) {}
    virtual void onWindowFocusOut(bool focused) {}
    virtual void onWindowCursorEnter() {}
    virtual void onWindowCursorLeave() {}
    virtual void onWindowKeyDown(int key, int modifier = 0) {}
    virtual void onWindowKeyUp(int key, int modifier = 0) {}
    virtual void onWindowMouseMove(double x, double y) {}
    virtual void onWindowMouseButtonDown(int button, int modifier = 0) {}
    virtual void onWindowMouseButtonUp(int button, int modifier = 0) {}
    virtual void onWindowScroll(double x, double y) {}

private:
    void buildView();
    virtual void handleEvent(const SDL_Event& evt);

private:
    Window window_;
    Input input_;
    Graphics::Renderer renderer_;
    Graphics::BulletDebugDrawer bullet_debug_drawer_;
    Sound::Space sound_space_;
    Console console_;
    Graphics::View views_[2];
    std::atomic_int current_view_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOCAL_H
