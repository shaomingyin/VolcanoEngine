//
//
#include <Volcano/Editor/SceneViewEditor.hpp>
#include <Volcano/Editor/SceneViewEditorFactory.hpp>

VOLCANO_EDITOR_BEGIN

SceneViewEditorFactory::SceneViewEditorFactory() {
    setMimeTypes({ VOLCANO_EDITOR_MIME_SCENE_FILE });
    setEditorCreator([] {
        return new SceneViewEditor;
    });
};

VOLCANO_EDITOR_END
