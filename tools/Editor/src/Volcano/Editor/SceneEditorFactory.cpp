//
//
#include <Volcano/Editor/SceneEditor.h>
#include <Volcano/Editor/SceneEditorFactory.h>

VOLCANO_EDITOR_BEGIN

SceneEditorFactory::SceneEditorFactory() {
    addMimeType(SCENE_FILE_MIMETYPE);
    setId(SceneEditor::Id);
    setDisplayName("Volcano Scene Editor");
    setEditorCreator([] {
        return new SceneEditor();
    });
}

VOLCANO_EDITOR_END
