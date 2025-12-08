//
//
#include <Volcano/Editor/WorldEditor.h>
#include <Volcano/Editor/WorldEditorFactory.h>

VOLCANO_EDITOR_BEGIN

WorldEditorFactory::WorldEditorFactory(Context& context) {
    addMimeType(WORLD_FILE_MIMETYPE);
    setId(WorldEditor::Id);
    setDisplayName("Volcano World Editor");
    setEditorCreator([&context] {
        return new WorldEditor(context);
    });
}

VOLCANO_EDITOR_END
