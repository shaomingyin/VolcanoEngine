//
//
#include <QApplication>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/MainWindow.hpp>

VOLCANO_EDITOR_BEGIN

static int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    initAppInfo("Editor");

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

VOLCANO_EDITOR_END

int main(int argc, char *argv[])
{
    return Volcano::Editor::main(argc, argv);
}
