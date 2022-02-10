//
//
#include <QApplication>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

static int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    setApplicationName("Editor");

    return EXIT_SUCCESS;
}

VOLCANO_EDITOR_END

int main(int argc, char *argv[])
{
    return Volcano::Editor::main(argc, argv);
}
