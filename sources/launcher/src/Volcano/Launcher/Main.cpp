//
//
#include <string>
#include <memory>
#include <thread>

#include <argh.h>
#include <GLFW/glfw3.h>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Launcher/Common.hpp>

VOLCANO_LAUNCHER_BEGIN

static int main(int argc, char *argv[])
{
    if (!glfwInit())
        return EXIT_FAILURE;
    
    ScopeGuard glfwGuard([] { glfwTerminate(); });

    GLFWwindow *mainWindow = glfwCreateWindow(800, 600, "Volcano", nullptr, nullptr);
    if (mainWindow == nullptr)
        return EXIT_FAILURE;

    ScopeGuard mainWindowGuard([=] { glfwDestroyWindow(mainWindow); });

    glfwMakeContextCurrent(mainWindow);

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();
        glfwSwapBuffers(mainWindow);
    }

    return EXIT_SUCCESS;
}

VOLCANO_LAUNCHER_END

int main(int argc, char *argv[])
{
    return Volcano::Launcher::main(argc, argv);
}
