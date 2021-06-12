//
//
#include <QCommandLineParser>

#include <Volcano/Editor/Application.hpp>

VOLCANO_EDITOR_BEGIN

Application::Application(int &argc, char *argv[]):
    QApplication(argc, argv)
{
}

Application::~Application(void)
{
}

bool Application::init(void)
{
    QCommandLineParser parser;

    parser.addPositionalArgument("game-dir", "Specifies the game root directory.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(*this);

    return true;
}

VOLCANO_EDITOR_END
