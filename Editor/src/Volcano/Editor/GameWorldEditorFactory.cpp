//
//
#include <memory>

#include <Volcano/Editor/GameWorldEditor.hpp>
#include <Volcano/Editor/GameWorldEditorFactory.hpp>

VOLCANO_EDITOR_BEGIN

GameWorldEditorFactory::GameWorldEditorFactory(void)
{
    setId("Editors.VolcanoWorldEditor");
    setMimeTypes({ "application/x-volcano-world" });
    setDisplayName("Volcano World Editor");
    setEditorCreator([] { return new GameWorldEditor(); });
}

GameWorldEditorFactory::~GameWorldEditorFactory(void)
{
}

VOLCANO_EDITOR_END
