//
//
#include <memory>

#include <QApplication>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

static int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setApplicationName("VolcanoEditor");

    qInfo("VolcanoEngine %s", VOLCANO_VERSION_STR);

    Graphics::initDefaultSettings();

    auto mainWindow = std::make_unique<MainWindow>();
    if (!mainWindow || !mainWindow->init()) {
        qCritical("Failed to create editor's main window.");
        return EXIT_FAILURE;
    }

    mainWindow->setWindowTitle("VolcanoEditor");
    mainWindow->show();

    return app.exec();
}

VOLCANO_EDITOR_END

int main(int argc, char *argv[])
{
    return Volcano::Editor::main(argc, argv);
}
