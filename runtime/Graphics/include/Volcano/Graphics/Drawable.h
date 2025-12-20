//
//
#ifndef VOLCANO_GRAPHICS_DRAWABLE_H
#define VOLCANO_GRAPHICS_DRAWABLE_H

#include <QObject>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Context.h>

VOLCANO_GRAPHICS_BEGIN

class Drawable: public QObject {
    Q_OBJECT

public:
    Drawable(Context& context, QObject* parent = nullptr);

public:
    Context& context() {
        return context_;
    }

    const Context& context() const {
        return context_;
    }

    bool isEnabled() const {
        return (flags_ & FlagEnabled);
    }

    bool isVisible() const {
        return (flags_ & FlagVisible);
    }

    float detailLevel() const {
        return detail_level_;
    }

    void update() {
        if (flags_ & FlagEnabled) {
            //tick(elapsed);
        }
    }

    void render() {
        if (isEnabled() && isVisible()) {
            draw();
        }
    }

    virtual void enable();
    virtual void disable();
    virtual void show();
    virtual void hide();
    virtual void setDetailLevel(float level);

protected:
    virtual void tick();
    virtual void draw();

private:
    enum {
        FlagEnabled = 0x1,
        FlagVisible = 0x2
    };

    Context& context_;
    int flags_;
    float detail_level_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DRAWABLE_H
