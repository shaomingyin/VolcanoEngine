//
//
#include <Volcano/Editor/SceneEditor.h>
#include <Volcano/Editor/SceneEditorFactory.h>

VOLCANO_EDITOR_BEGIN

SceneEditorFactory::SceneEditorFactory(void)
{
    // addMimeType("qml???");
}

SceneEditorFactory::~SceneEditorFactory(void)
{
}

Core::IEditor *SceneEditorFactory::createEditor(void)
{
    // new SceneEditor
    return nullptr;
}

VOLCANO_EDITOR_END
