//
//
#include <Volcano/Editor/SceneTreeEditor.hpp>

VOLCANO_EDITOR_BEGIN

SceneTreeEditor::SceneTreeEditor() {
    layout_.setSpacing(0);
    layout_.setContentsMargins(0, 0, 0, 0);
    layout_.addWidget(&scene_selector_);
    layout_.addWidget(&scene_tree_view_);
}

Core::IDocument *SceneTreeEditor::document() const {
    return nullptr;
}

QWidget *SceneTreeEditor::toolBar() {
    return nullptr;
}

VOLCANO_EDITOR_END
