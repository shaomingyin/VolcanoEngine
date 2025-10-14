//
//
#ifndef VOLCANO_GUI_CONTEXT_H
#define VOLCANO_GUI_CONTEXT_H

#include <chrono>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

class Context {
public:
    Context(int width, int height);
    virtual ~Context();

public:
    void update(std::chrono::high_resolution_clock::duration elapsed);
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_CONTEXT_H
