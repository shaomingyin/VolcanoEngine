//
//
#include <string>
#include <memory>

#include <argh.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Boot/Common.hpp>
#include <Volcano/Boot/Engine.hpp>

VOLCANO_BOOT_BEGIN

static void pollEvents(uv_timer_t *p)
{
    auto engine = reinterpret_cast<Engine *>(p->data);
    VOLCANO_ASSERT(engine != nullptr);

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type != SDL_QUIT)
            engine->handleEvent(event);
        else
            uv_stop(p->loop);
    }
}

static int run(uv_loop_t *loop)
{
    VOLCANO_ASSERT(loop != nullptr);

    VOLCANO_LOGI("Creating engine...");

    auto engine = std::make_unique<Engine>(loop);
    if (!engine || !engine->start()) {
        VOLCANO_LOGE("Failed to init engine.");
        return EXIT_FAILURE;
    }

    uv_timer_t pollEventsTimer;
    uv_timer_init(loop, &pollEventsTimer);
    uv_timer_start(&pollEventsTimer, &pollEvents, 0, 15);
    pollEventsTimer.data = engine.get();
    
    VOLCANO_LOGI("Start main loop...");
    uv_run(loop, UV_RUN_DEFAULT);

    return EXIT_SUCCESS;
}

VOLCANO_BOOT_END

static void volcanoPrintHelp(void)
{
}

static void volcanoPrintVersion(void)
{
}

static void volcanoLogOutputFunction(void *userdata, int category, SDL_LogPriority priority, const char *message)
{
    FILE *fp;

    if (priority == SDL_LOG_PRIORITY_ERROR || priority == SDL_LOG_PRIORITY_CRITICAL)
        fp = stderr;
    else
        fp = stdout;

    fprintf(fp, "[%06d] %08d: %s\n", SDL_ThreadID(), SDL_GetTicks(), message);
}

int main(int argc, char *argv[])
{
    SDL_LogSetOutputFunction(&volcanoLogOutputFunction, NULL);

    argh::parser cmdline;
    cmdline.add_params({ "-m", "--mode" });
    cmdline.parse(argc, argv);

#ifdef VOLCANO_DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
    if (cmdline[{ "-d", "--debug"}])
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

    if (cmdline[{ "-h", "--help"}]) {
        volcanoPrintHelp();
        return EXIT_SUCCESS;
    }

    if (cmdline[{ "-v", "--version"}]) {
        volcanoPrintVersion();
        return EXIT_SUCCESS;
    }

    VOLCANO_LOGI("VolcanoEngine - %d.%d.%d",
        VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);

    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret != 0) {
        VOLCANO_LOGE("Failed to init SDL.");
        return EXIT_FAILURE;
    }

    Volcano::ScopeGuard sdlGuard([] { SDL_Quit(); });

    return Volcano::Boot::run(uv_default_loop());
}
