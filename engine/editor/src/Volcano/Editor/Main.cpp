//
//
#include <QApplication>

#include <Volcano/Qml/Common.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

static int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Qml::registerAll();

    return app.exec();
}

VOLCANO_EDITOR_END

int main(int argc, char *argv[])
{
    return Volcano::Editor::main(argc, argv);
}
