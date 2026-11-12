#ifndef VOLCANO_GUI_PLATFORM_H
#define VOLCANO_GUI_PLATFORM_H

#include <filesystem>
#include <string>
#include <string_view>

#include <Volcano/Color.h>
#include <Volcano/Rect.h>
#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Style.h>

VOLCANO_GUI_BEGIN

// Host services used by GUI on its owning thread. Input events are delivered to Document.
class Platform {
public:
    enum class Cursor {
        Arrow,
        Text,
        Hand,
        Crosshair,
        Move,
        ResizeHorizontal,
        ResizeVertical,
        ResizeDiagonalForward,
        ResizeDiagonalBackward,
        Forbidden,
        Hidden
    };

    virtual ~Platform() = default;

    // The host keeps the configuration alive during GUI calls and requests redraws after changes.
    virtual const Style& style() const noexcept = 0;

    virtual void setCursor(Cursor cursor) = 0;

    // Clipboard text is UTF-8; an empty string also represents no available text.
    virtual std::string clipboardText() = 0;
    virtual void setClipboardText(std::string_view text) = 0;

    virtual void startTextInput() = 0;
    virtual void stopTextInput() = 0;
    // Document-local logical coordinates; the host maps them to the input method's screen position.
    virtual void setTextInputRect(const Rectf& rect) = 0;

    // The host must deliver a pointer cancellation event if capture is lost.
    virtual void setPointerCapture(bool enabled) = 0;

    virtual Sizef measure(const std::filesystem::path& font, float pixels, std::string_view text) = 0;
};

VOLCANO_GUI_END

#endif
