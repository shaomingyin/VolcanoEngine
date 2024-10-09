//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Driver.h>
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer();
    virtual ~Renderer();

public:
    virtual bool init(int width, int height);

    Driver* driver() {
        return driver_;
    }

    bool setDriver(Driver* p);

public:
    bool begin();
    void end();
    void update(std::chrono::steady_clock::duration elapsed);

private:
    int widht_;
    int height_;
    Driver* driver_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
