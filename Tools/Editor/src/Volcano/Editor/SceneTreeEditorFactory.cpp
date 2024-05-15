//
//
#include <Volcano/Editor/SceneTreeEditor.hpp>
#include <Volcano/Editor/SceneTreeEditorFactory.hpp>

VOLCANO_EDITOR_BEGIN

SceneTreeEditorFactory::SceneTreeEditorFactory() {
    setMimeTypes({ VOLCANO_EDITOR_MIME_SCENE_FILE });
    setEditorCreator([] {
        return new SceneTreeEditor;
    });
}

VOLCANO_EDITOR_END
