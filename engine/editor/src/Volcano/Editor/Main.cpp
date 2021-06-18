//
//
#include <QScopedPointer>
#include <QApplication>

#include <Volcano/Init.hpp>
#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

static int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setApplicationName("Editor");

    if (!init()) {
        qFatal("Failed to init system.");
        return EXIT_FAILURE;
    }

    QScopedPointer<MainWindow> mainWindow(new MainWindow());
    if (!mainWindow || !mainWindow->init()) {
        qFatal("Failed to create main window.");
        return EXIT_FAILURE;
    }

    mainWindow->resize(800, 600);
    mainWindow->show();

    return app.exec();
}

VOLCANO_EDITOR_END

int main(int argc, char *argv[])
{
    return Volcano::Editor::main(argc, argv);
}
