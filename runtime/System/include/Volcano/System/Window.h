//
//
#ifndef VOLCANO_SYSTEM_WINDOW_H
#define VOLCANO_SYSTEM_WINDOW_H

#include <Volcano/Graphics/Target.h>
#include <Volcano/Gui/MenuBar.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class Window: public Graphics::Target {
public:
    Window(const std::string& title, int x, int y, int w, int h);
    virtual ~Window();

public:
    SDL_Window* handle() {
        return handle_;
    }

    SDL_GLContext glContext() {
        return gl_context_;
    }

    Uint32 id() const {
        return id_;
    }

    int x() const {
        return x_;
    }

    int y() const {
        return y_;
    }

    void move(int x, int y) {
        SDL_SetWindowPosition(handle_, x, y);
    }

    bool isVisible() const {
        return bool(state_ & StateVisible);
    }

    void show() {
        SDL_ShowWindow(handle_);
    }

    void hide() {
        SDL_HideWindow(handle_);
    }

    void setTitle(const std::string& v) {
        SDL_SetWindowTitle(handle_, v.c_str());
    }

    void resize(const Eigen::Vector2i& v) {
        resize(v.x(), v.y());
    }

    Gui::MenuBar& menuBar() {
        return menu_bar_;
    }

    void resize(int w, int h) override;
    bool makeCurrent() override;
    void swapBuffers();
    virtual void handleEvent(const SDL_Event& evt);
    void toggleFullScreen();

private:
    void handleWindowEvent(const SDL_Event& evt);
    void handleKeyEvent(const SDL_Event& evt);

private:
    enum {
        StateVisible = 0x1,
        StateFullScreen = 0x2
    };

    int state_;
    SDL_Window* handle_;
    Uint32 id_;
    int x_;
    int y_;
    SDL_GLContext gl_context_;
    GL3WProcs gl3w_;
    ImGuiContext* imgui_;
    Gui::MenuBar menu_bar_;
    bool show_gui_;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_WINDOW_H
