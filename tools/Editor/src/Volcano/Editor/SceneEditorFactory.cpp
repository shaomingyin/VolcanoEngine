//
//
#include <Volcano/Editor/SceneEditor.h>
#include <Volcano/Editor/SceneEditorFactory.h>

VOLCANO_EDITOR_BEGIN

SceneEditorFactory::SceneEditorFactory(Context& context) {
    addMimeType(SCENE_FILE_MIMETYPE);
    setId(SceneEditor::Id);
    setDisplayName("Volcano Scene Editor");
    setEditorCreator([&context] {
        return new SceneEditor(context);
    });
}

VOLCANO_EDITOR_END
