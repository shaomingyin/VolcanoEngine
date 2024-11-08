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
    bool isPhysicsDebugEnabled() const {
        return (scene().physicsDebugDrawer() != nullptr);
    }

    void enablePhysicsDebug() {
        scene().setPhysicsDebugDrawer(&bullet_debug_drawer_);
    }

    void disablePhysicsDebug() {
        scene().setPhysicsDebugDrawer(nullptr);
    }

protected:
    void handleEvent(const SDL_Event& evt);
    bool beginFrame() override;
    void endFrame() override;
    void loadingFrame(Duration elapsed) override;
    void readyFrame(Duration elapsed) override;
	void playingFrame(Duration elapsed) override;
    void pausedFrame(Duration elapsed) override;
    void stoppingFrame(Duration elapsed) override;
    void errorFrame(Duration elapsed) override;
    void loadEntity(World::Entity ent) override;

private:
    void buildView();

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
