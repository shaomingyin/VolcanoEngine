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

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/VM/Kernel.hpp>
#include <Volcano/Boot/Common.hpp>

VOLCANO_BOOT_BEGIN

static void pollEvents(uv_timer_t *timer)
{
    SDL_Event evt;
    auto kernel = reinterpret_cast<VM::Kernel *>(timer->data);
    VOLCANO_ASSERT(kernel != nullptr);

    while (SDL_PollEvent(&evt)) {
        if (VOLCANO_LIKELY(evt.type != SDL_QUIT))
            kernel->postEvent(evt);
        else
            uv_stop(timer->loop);
    }
}

static void logOutput(void *data, int, SDL_LogPriority priority, const char *message)
{
    FILE *fp = stdout;
    const char *prefix = "";
    auto kernel = *reinterpret_cast<VM::Kernel **>(data);

    if (kernel != nullptr) {
        if (std::this_thread::get_id() == kernel->thread().get_id())
            prefix = "VM: ";
    }

    if (priority == SDL_LOG_PRIORITY_ERROR ||
        priority == SDL_LOG_PRIORITY_CRITICAL ||
        priority == SDL_LOG_PRIORITY_WARN)
        fp = stderr;

    fprintf(fp, "%s%s\n", prefix, message);
}

static void printHelp(void)
{
    VOLCANO_LOGI("Usage:");
    VOLCANO_LOGI("  sge [options] <root directory>");
    VOLCANO_LOGI(" ");
    VOLCANO_LOGI("options:");
    VOLCANO_LOGI("  --init=<filename>");
    VOLCANO_LOGI("    Specified the initrc, default is '/init.lua'.");
    VOLCANO_LOGI(" ");
    VOLCANO_LOGI("  --debug");
    VOLCANO_LOGI("    Set log level to debug.");
    VOLCANO_LOGI(" ");
    VOLCANO_LOGI("  -v, --version");
    VOLCANO_LOGI("    Show version.");
    VOLCANO_LOGI(" ");
    VOLCANO_LOGI("  -h, --help");
    VOLCANO_LOGI("    Show this message.");
}

int main(int argc, char *argv[])
{
    VM::Kernel *kernel = nullptr;
    SDL_LogSetOutputFunction(logOutput, &kernel);

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
            VOLCANO_LOGE("Too many roots.");
            return EXIT_FAILURE;
        }
        root = std::filesystem::canonical(cmdLine(1).str()).string();
        if (!std::filesystem::is_directory(root)) {
            VOLCANO_LOGE("Root must be a directory.");
            return EXIT_FAILURE;
        }
    }

    int ret = SDL_Init(SDL_INIT_EVERYTHING);
    if (ret < 0) {
        VOLCANO_LOGE("Failed to init SDL.");
        return EXIT_FAILURE;
    }

    VOLCANO_SCOPE_EXIT(r1) {
        SDL_Quit();
    };

    std::unique_ptr<VM::Kernel> p(new VM::Kernel(uv_default_loop()));
    kernel = p.get();
    if (!p || !p->start(root, init)) {
        VOLCANO_LOGE("Failed to start VM.");
        return EXIT_FAILURE;
    }

    uv_timer_t pollEventsTimer;
    uv_timer_init(uv_default_loop(), &pollEventsTimer);
    uv_timer_start(&pollEventsTimer, &pollEvents, 0, 15);
    pollEventsTimer.data = kernel;

    VOLCANO_SCOPE_EXIT(r2) {
        uv_close_sync(&pollEventsTimer);
    };

    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    return EXIT_SUCCESS;
}

VOLCANO_BOOT_END

int main(int argc, char *argv[])
{
    return Volcano::Boot::main(argc, argv);
}
