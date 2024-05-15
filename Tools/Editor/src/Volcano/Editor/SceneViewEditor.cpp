//
//
#include <Volcano/Editor/SceneViewEditor.hpp>

VOLCANO_EDITOR_BEGIN

SceneViewEditor::SceneViewEditor() {
};

Core::IDocument *SceneViewEditor::document() const {
    return &document_;
}

QWidget *SceneViewEditor::toolBar() {
    return nullptr;
}

VOLCANO_EDITOR_END
