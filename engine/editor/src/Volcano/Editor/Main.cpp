//
//
#include <QScopedPointer>
#include <QApplication>

#include <Volcano/Game/Init.hpp>
#include <Volcano/Graphics/Init.hpp>
#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

static int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setApplicationName("Editor");

    QScopedPointer<MainWindow> mainWindow(new MainWindow());
    if (!mainWindow || !mainWindow->init()) {
        qFatal("Failed to create main window.");
        return EXIT_FAILURE;
    }

    Game::init();
    Graphics::init();

    mainWindow->setWindowTitle("Volcano Editor");
    mainWindow->show();

    return app.exec();
}

VOLCANO_EDITOR_END

int main(int argc, char *argv[])
{
    return Volcano::Editor::main(argc, argv);
}
