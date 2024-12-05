//
//
#include <nanovg_ctx.h>

#define NANOVG_GL3_IMPLEMENTATION
#include <GL/gl3w.h>
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>

namespace nvg {
    NVGcontext* current;

    Context* createContext() {
        return nvgCreateGL3(0);
    }
}
