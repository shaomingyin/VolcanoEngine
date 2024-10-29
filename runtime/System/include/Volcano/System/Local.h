//
//
#ifndef VOLCANO_SYSTEM_LOCAL_H
#define VOLCANO_SYSTEM_LOCAL_H

#include <atomic>

#include <taskflow/taskflow.hpp>

#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Renderer.h>
#include <Volcano/System/Common.h>
#include <Volcano/System/Base.h>

VOLCANO_SYSTEM_BEGIN

class Local: public Base {
public:
	Local(const std::filesystem::path& root, const std::filesystem::path& init);
	virtual ~Local();

public:
    void init() override;
    void feedEvent(const SDL_Event& evt);

protected:
	void handleEvent(const SDL_Event& evt) override;
	void frame(Duration elapsed) override;

private:
    void buildGraphicsView();

private:
    enum {
        FlagWindowVisible = 0x1
    };

    int flags_;
    SDL_Window* window_;
    Uint32 window_id_;
    SDL_GLContext gl_context_;
    Graphics::Renderer graphics_renderer_;
    Graphics::View views_[2];
    std::atomic_int current_view_;
    tf::Executor executor_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_LOCAL_H
