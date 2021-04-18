//
//
#include <string>
#include <string_view>
#include <memory>
#include <filesystem>

#include <argh.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/Engine.hpp>

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

static void volcanoPollEvents(uv_timer_t *p)
{
    auto engine = reinterpret_cast<Volcano::System::Engine *>(p->data);
    VOLCANO_ASSERT(engine != nullptr);

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type != SDL_QUIT)
            engine->handleEvent(event);
        else
            uv_stop(p->loop);
    }
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

    std::string rootPath = std::filesystem::current_path().u8string();
    if (cmdline.size() > 1) {
        if (cmdline.size() != 2) {
            volcanoPrintHelp();
            return EXIT_FAILURE;
        }
        rootPath = cmdline[1];
    }

    VOLCANO_LOGI("VM root: %s", rootPath.c_str());

    if (!std::filesystem::is_directory(rootPath)) {
        VOLCANO_LOGE("Invalid root path '%s', not a directory.", rootPath.c_str());
        return EXIT_FAILURE;
    }

    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret != 0) {
        VOLCANO_LOGE("Failed to init SDL.");
        return EXIT_FAILURE;
    }

    Volcano::ScopeGuard sdlGuard([] { SDL_Quit(); });

    VOLCANO_LOGI("Initializing engine...");

    uv_loop_t *loop = uv_default_loop();
    if (loop == nullptr) {
        VOLCANO_LOGE("Failed to init libuv.");
        return EXIT_FAILURE;
    }

    auto engine = std::make_unique<Volcano::System::Engine>(loop, rootPath);
    if (!engine || !engine->start()) {
        VOLCANO_LOGE("Failed to init engine.");
        return EXIT_FAILURE;
    }

    uv_timer_t pollEventsTimer;
    uv_timer_init(loop, &pollEventsTimer);
    uv_timer_start(&pollEventsTimer, &volcanoPollEvents, 0, 15);
    pollEventsTimer.data = engine.get();

    VOLCANO_LOGI("Start main loop...");
    uv_run(loop, UV_RUN_DEFAULT);

    return EXIT_SUCCESS;
}
