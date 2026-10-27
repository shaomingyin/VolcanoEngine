//
//
#include <Volcano/Editor/SceneEditor.h>

VOLCANO_EDITOR_BEGIN

const Utils::Id SceneEditor::Id = "VolcanoEditor.SceneEditor";

SceneEditor::SceneEditor(Context& context)
    : context_(context)
    , document_(std::make_shared<SceneDocument>()) {
    setDuplicateSupported(false);
    setWidget(&viewport_);
}

Core::IDocument* SceneEditor::document() const {
    return document_.get();
}

QWidget* SceneEditor::toolBar() {
    return &toolbar_;
}

VOLCANO_EDITOR_END
