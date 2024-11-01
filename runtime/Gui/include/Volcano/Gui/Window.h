//
//
#ifndef VOLCANO_GUI_WINDOW_H
#define VOLCANO_GUI_WINDOW_H

#include <string>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Context.h>

VOLCANO_GUI_BEGIN

class Window: public Context {
public:
    Window(NVGcontext* nvg);
    ~Window() override = default;

public:
    void render();
    NVGcontext* nvg() override;
    virtual void handleEvent(const SDL_Event& evt);

    const std::string& title() const {
        return title_;
    }

    void setTitle(const std::string& v) {
        title_ = v;
    }

    void setTitle(std::string&& v) {
        title_ = std::move(v);
    }

private:
    NVGcontext* nvg_;
    std::string title_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WINDOW_H
