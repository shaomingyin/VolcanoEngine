//
//
#include <QScopedPointer>
#include <QCommandLineParser>
#include <QApplication>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

static int Main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setOrganizationName("VolcanoEngine");
    QApplication::setApplicationDisplayName("Volcano Editor");
    QApplication::setApplicationName("VolcanoEditor");

    QScopedPointer<MainWindow> mainWindow(new MainWindow);
    if (!mainWindow || !mainWindow->init())
    {
        return EXIT_FAILURE;
    }

    mainWindow->show();

    return QApplication::exec();
}

VOLCANO_EDITOR_END

int main(int argc, char *argv[])
{
    return Volcano::Editor::Main(argc, argv);
}
