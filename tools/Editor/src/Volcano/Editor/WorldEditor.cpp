//
//
#include <Volcano/Editor/WorldEditor.h>

VOLCANO_EDITOR_BEGIN

const Utils::Id WorldEditor::Id = "VolcanoEditor.WorldEditor";

WorldEditor::WorldEditor(Context& context)
    : context_(context)
    , document_(std::make_shared<WorldDocument>()) {
    setDuplicateSupported(false);
    setWidget(&viewport_);
}

Core::IDocument* WorldEditor::document() const {
    return document_.get();
}

QWidget* WorldEditor::toolBar() {
    return &toolbar_;
}

VOLCANO_EDITOR_END
