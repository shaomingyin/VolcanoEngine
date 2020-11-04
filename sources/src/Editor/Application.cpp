//
//
#include <Volcano/Launcher/Application.hpp>

VOLCANO_LAUNCHER_BEGIN

static int fakeArgc = 1;
static char *fakeArgv[] = { "VolcanoLauncher" };

Application::Application(const QStringList &args):
    QGuiApplication(fakeArgc, fakeArgv),
    m_args(args)
{
}

Application::~Application(void)
{
}

VOLCANO_LAUNCHER_END
