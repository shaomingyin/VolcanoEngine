//
//
#include <string>
#include <string_view>
#include <memory>
#include <mutex>
#include <filesystem>

#include <argh.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/System/Common.hpp>
#include <Volcano/System/Engine.hpp>

VOLCANO_SYSTEM_BEGIN

static void printHelp(void)
{
}

static void printVersion(void)
{
}

static void logOutput(void *data, int category, SDL_LogPriority priority, const char *message)
{
#ifdef VOLCANO_DEBUG
    FILE *fp;
    if (priority == SDL_LOG_PRIORITY_CRITICAL || priority == SDL_LOG_PRIORITY_ERROR)
        fp = stderr;
    else
        fp = stdout;
#else
    static FILE *fp = nullptr
    static std::mutex mutex;

    {
        std::lock_guard<std::mutex> locker(mutex);
        if (fp == nullptr) {
            fp = f
        }
    }
#endif

    static const char indTable[] = { 'V', 'D', 'I', 'W', 'E', 'F' };

    char ind = '?';

    if (SDL_LOG_PRIORITY_VERBOSE <= priority && priority < SDL_NUM_LOG_PRIORITIES)
        ind = indTable[priority - SDL_LOG_PRIORITY_VERBOSE];

    fprintf(fp, "[%06d] %c %08d: %s\n", SDL_ThreadID(), ind, SDL_GetTicks(), message);
}

static int Run(int argc, char *argv[])
{
#ifdef VOLCANO_DEBUG
    SDL_SetMainReady();
#endif

    argh::parser cmdline;
    cmdline.add_params({ "-m", "--mode" });
    cmdline.parse(argc, argv);

    SDL_LogSetOutputFunction(&logOutput, nullptr);

#ifdef VOLCANO_DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#else
    if (cmdline[{ "-d", "--debug"}])
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_DEBUG);
#endif

    if (cmdline[{ "-h", "--help"}]) {
        printHelp();
        return EXIT_SUCCESS;
    }

    if (cmdline[{ "-v", "--version"}]) {
        printVersion();
        return EXIT_SUCCESS;
    }

    VOLCANO_LOGI("VolcanoEngine - %d.%d.%d",
        VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);

    std::string rootPath = std::filesystem::current_path().u8string();
    if (cmdline.size() > 1) {
        if (cmdline.size() != 2) {
            printHelp();
            return EXIT_FAILURE;
        }
        rootPath = cmdline[1];
    }

    VOLCANO_LOGI("VM root: %s", rootPath.c_str());

    if (!std::filesystem::is_directory(rootPath)) {
        VOLCANO_LOGE("Invalid root path '%s', not a directory.", rootPath.c_str());
        return EXIT_FAILURE;
    }

    VOLCANO_LOGI("Initializing physfs...");

    if (!PHYSFS_init("volcano")) {
        VOLCANO_LOGE("Failed to init physfs.");
        return EXIT_FAILURE;
    }

    ScopeGuard physfsGuard([] { PHYSFS_deinit(); });

    if (!PHYSFS_setWriteDir(rootPath.c_str())) {
        VOLCANO_LOGE("Failed to set write dir to '%s'.", rootPath.c_str());
        return EXIT_FAILURE;
    }

    if (!PHYSFS_mount(rootPath.c_str(), "/", 0)) {
        VOLCANO_LOGE("Failed to mount root path '%s'.", rootPath.c_str());
        return EXIT_FAILURE;
    }

    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret != 0) {
        VOLCANO_LOGE("Failed to init SDL.");
        return EXIT_FAILURE;
    }

    ScopeGuard sdlGuard([] { SDL_Quit(); });

    VOLCANO_LOGI("Initializing GLEX...");
    if (!glexInit()) {
        VOLCANO_LOGE("Failed to init GLEX.");
        return EXIT_FAILURE;
    }

    ScopeGuard glexGuard([] { glexShutdown(); });

    glexEnableLog();

#ifdef VOLCANO_DEBUG
    glexLogLevel(GLEX_LOG_LEVEL_DEBUG);
#else
    glexLogLevel(GLEX_LOG_LEVEL_INFO);
#endif

    VOLCANO_LOGI("Initializing engine...");

    uv_loop_t *loop = uv_default_loop();
    if (loop == nullptr) {
        VOLCANO_LOGE("Failed to init libuv.");
        return EXIT_FAILURE;
    }

    auto engine = std::make_unique<Engine>(loop);
    if (!engine || !engine->init()) {
        VOLCANO_LOGE("Failed to initialize engine.");
        return EXIT_FAILURE;
    }

    uv_timer_t pollEventsTimer;
    uv_timer_init(loop, &pollEventsTimer);
    uv_timer_start(&pollEventsTimer, [] (uv_timer_t *p) {
        auto engine = reinterpret_cast<Engine *>(p->data);
        SDL_Event event;
        while (SDL_PollEvent(&event))
            engine->handleEvent(event);
    }, 0, 15);
    pollEventsTimer.data = engine.get();
    ScopeGuard pollEventsGuard([&pollEventsTimer] { uvSyncClose(&pollEventsTimer); });

    VOLCANO_LOGI("Start main loop...");
    uv_run(loop, UV_RUN_DEFAULT);

    return EXIT_SUCCESS;
}

VOLCANO_SYSTEM_END

int main(int argc, char *argv[])
{
    return Volcano::System::Run(argc, argv);
}
