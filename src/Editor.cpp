//
//
#include <Volcano/Editor.h>

VOLCANO_BEGIN

Editor::Editor(void)
{
}

Editor::~Editor(void)
{

}

bool Editor::init(const QUrl &url)
{
    if (!Launcher::init(url))
        return false;

    setWindowTitle("Volcano Editor");

    return true;
}

VOLCANO_END
