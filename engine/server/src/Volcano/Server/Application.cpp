//
//
#include <QCommandLineParser>

#include <Volcano/Server/Application.hpp>

VOLCANO_SERVER_BEGIN

Application::Application(int &argc, char *argv[]):
    QCoreApplication(argc, argv)
{
}

Application::~Application(void)
{
}

bool Application::init(void)
{
    QCommandLineParser cmdline;

    cmdline.addPositionalArgument("game-dir", "Specifies the game root directory.");
    cmdline.addHelpOption();
    cmdline.addVersionOption();
    cmdline.process(*this);

    return true;
}

VOLCANO_SERVER_END
