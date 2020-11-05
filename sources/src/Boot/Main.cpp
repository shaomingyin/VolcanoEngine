//
//
#include <cstring>

#include <QString>
#include <QStringList>
#include <QScopedPointer>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QCoreApplication>

#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/DirectionalLight.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Light.hpp>
#include <Volcano/Game/Model.hpp>
#include <Volcano/Game/OrthoCamera.hpp>
#include <Volcano/Game/PerpectiveCamera.hpp>
#include <Volcano/Game/PointLight.hpp>
#include <Volcano/Game/SpotLight.hpp>
#include <Volcano/Game/World.hpp>

#include <Volcano/Player/Main.hpp>
#include <Volcano/Editor/Main.hpp>
#include <Volcano/Server/Main.hpp>

#include <Volcano/Boot/Common.hpp>

VOLCANO_BOOT_BEGIN

static void printHelp(void)
{
    qInfo("Usage:");
    qInfo("  Volcano <mode> [parameters...]\n");
    qInfo("Modes:");
    qInfo("  launch <game path>");
    qInfo("    Launch a game in 'game path' to play.\n");
    qInfo("  edit <game path>");
    qInfo("    Edit a game in 'game path'.\n");
    qInfo("  serve <game path>");
    qInfo("    Start a game server for game in 'game path.'\n");
    qInfo("  help");
    qInfo("    Print this informations.\n");
    qInfo("  version");
    qInfo("    Print version.");
}

static void printVersion(void)
{
    qInfo("%d.%d.%d", VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);
}

static int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printHelp();
        return EXIT_FAILURE;
    }

    int subArgc = argc - 1;
    char **subArgv = (char **)malloc(sizeof(char *) * subArgc);

    subArgv[0] = strdup(argv[0]);

    for (int i = 1; i < subArgc; ++i)
        subArgv[i] = strdup(argv[i + 1]);

    int ret = EXIT_FAILURE;

    if (strcmp(argv[1], "play") == 0)
        ret = Player::main(subArgc, subArgv);
    else if (strcmp(argv[1], "edit") == 0)
        ret = Editor::main(subArgc, subArgv);
    else if (strcmp(argv[1], "serve") == 0)
        ret = Server::main(subArgc, subArgv);
    else if (strcmp(argv[1], "help") == 0)
        printHelp();
    else if (strcmp(argv[1], "version") == 0)
        printVersion();
    else
        qFatal("Unknown subcommand '%s'.", argv[1]);

    for (int i = 0; i < subArgc; ++i)
        free(subArgv[i]);

    free(subArgv);

    return ret;
}

VOLCANO_BOOT_END

int main(int argc, char *argv[])
{
    return Volcano::Boot::main(argc, argv);
}
