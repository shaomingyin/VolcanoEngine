//
//
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <memory>
#include <filesystem>

#include <argh.h>
#include <uv.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <Volcano/VM/Kernel.hpp>

struct SDLInitializer {
    int result;
    SDLInitializer(Uint32 flags): result(SDL_Init(flags)) { }
    ~SDLInitializer(void) { if (result == 0) SDL_Quit(); }
    operator bool(void) const { return (result == 0); }
};

static Volcano::VM::Kernel *vm;

static void pollEvents(uv_timer_t *p)
{
    SDL_Event evt;

    while (SDL_PollEvent(&evt)) {
        if (evt.type == SDL_QUIT) {
            if (vm != nullptr)
                vm->stop();
            uv_stop(p->loop);
        } else {
            if (vm != nullptr)
                vm->postEvent(evt);
        }
    }
}

static void printHelp(void)
{
    VOLCANO_LOGI("usage:");
    VOLCANO_LOGI("  sge [options] <root directory>");
    VOLCANO_LOGI(" ");
    VOLCANO_LOGI("options:");
    VOLCANO_LOGI("  --init=<filename>");
    VOLCANO_LOGI("    specified the initrc, default is '/init.lua'.");
    VOLCANO_LOGI(" ");
    VOLCANO_LOGI("  --debug");
    VOLCANO_LOGI("    set log level to debug.");
    VOLCANO_LOGI(" ");
    VOLCANO_LOGI("  -v, --version");
    VOLCANO_LOGI("    show version.");
    VOLCANO_LOGI(" ");
    VOLCANO_LOGI("  -h, --help");
    VOLCANO_LOGI("    show this message.");
}

static void logOutput(void *data, int, SDL_LogPriority priority, const char *message)
{
    FILE *fp = stdout;
    const char *prefix = "";

    if (vm != nullptr) {
        if (std::this_thread::get_id() == vm->thread().get_id())
            prefix = "vm: ";
    }

    if (priority == SDL_LOG_PRIORITY_ERROR ||
        priority == SDL_LOG_PRIORITY_CRITICAL ||
        priority == SDL_LOG_PRIORITY_WARN)
        fp = stderr;

    fprintf(fp, "%s%s\n", prefix, message);
}

int main(int argc, char *argv[])
{
    SDL_LogSetOutputFunction(logOutput, nullptr);

#ifdef VOLCANO_DEBUG
    SDL_LogSetPriority(VOLCANO_LOG_CATEGORY, SDL_LOG_PRIORITY_DEBUG);
#endif

    argh::parser cmdLine(argc, argv);

    if (cmdLine[{ "--debug" }])
        SDL_LogSetPriority(VOLCANO_LOG_CATEGORY, SDL_LOG_PRIORITY_DEBUG);

    if (cmdLine[{ "-h", "--help" }]) {
        printHelp();
        return EXIT_SUCCESS;
    }

    VOLCANO_LOGI("Volcano v%d.%d.%d",
        VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);

    if (cmdLine[{ "-v", "--version" }])
        return EXIT_SUCCESS;

    std::string init = "/init.lua";
    cmdLine({ "--init" }) >> init;

    std::string root = std::filesystem::current_path().string();
    if (cmdLine.size() > 1) {
        if (cmdLine.size() != 2) {
            VOLCANO_LOGE("too many roots.");
            return EXIT_FAILURE;
        }
        root = std::filesystem::canonical(cmdLine(1).str()).string();
        if (!std::filesystem::is_directory(root)) {
            VOLCANO_LOGE("root must be a directory.");
            return EXIT_FAILURE;
        }
    }

    SDLInitializer sdl(SDL_INIT_EVERYTHING);
    if (!sdl) {
        VOLCANO_LOGE("failed to init SDL.");
        return EXIT_FAILURE;
    }

    std::unique_ptr<Volcano::VM::Kernel> p(new Volcano::VM::Kernel(uv_default_loop()));
    if (!p || !p->start(root, init)) {
        VOLCANO_LOGE("failed to start vm.");
        return EXIT_FAILURE;
    }

    vm = p.get();

    uv_timer_t poll_events_timer;
    uv_timer_init(uv_default_loop(), &poll_events_timer);
    uv_timer_start(&poll_events_timer, &pollEvents, 0, 15);

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    uv_timer_stop(&poll_events_timer);

    vm = nullptr;

    return EXIT_SUCCESS;
}
