//
//
#define NANOVG_GL3_IMPLEMENTATION

#include <Volcano/Error.h>
#include <Volcano/ScopeGuard.h>
#include <Volcano/System/Local.h>

#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>

VOLCANO_SYSTEM_BEGIN

Local::Local(const std::filesystem::path& root, const std::filesystem::path& init)
    : Base(root, init)
    , window_(
        settings().get<std::string>("/local/title", "Untitled"),
        settings().get<int>("/local/width", 1024),
        settings().get<int>("/local/height", 768))
    , input_(window_.id())
    , nvg_(nvgCreateGL3(0))
    , renderer_(
        GL3WGetProcAddressProc(SDL_GL_GetProcAddress),
        window_.width(),
        window_.height())
    , hud_(nvg_)
    , console_(nvg_)
    , current_view_(0) {
    if (nvg_ == nullptr) {
        throw Error(Errc::OutOfResource);
    }
}

Local::~Local() {
    window_.makeCurrent();
    nvgDeleteGL3(nvg_);
}

void Local::handleEvent(const SDL_Event& evt) {
    window_.handleEvent(evt);
    input_.handleEvent(evt);
}

void Local::frame(Duration elapsed) {
	Base::frame(elapsed);
    sound_space_.frame(elapsed);

    if (window_.beginDraw()) {
        {
            auto end_draw = scopeGuard([this] {
                window_.endDraw();
            });
            
            buildView();
            renderer_.render(views_[current_view_], elapsed);

            drawPhysicsDebug();
            hud_.render();
            console_.render();
        }

        //int exp = current_view_;
        //while (current_view_.compare_exchange_strong(exp, !current_view_));
    }
}

void Local::buildView() {
    //int exp = current_view_;
    //while (current_view_.compare_exchange_strong(exp, !current_view_));
}

VOLCANO_SYSTEM_END
