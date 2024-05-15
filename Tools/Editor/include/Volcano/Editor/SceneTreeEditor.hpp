//
//
#ifndef VOLCANO_EDITOR_SCENETREEEDITOR_HPP
#define VOLCANO_EDITOR_SCENETREEEDITOR_HPP

#include <QList>
#include <QToolButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QTreeView>
#include <QWidget>

#include <coreplugin/editormanager/ieditor.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class SceneTreeEditor: public Core::IEditor {
    Q_OBJECT

public:
    SceneTreeEditor();

public:
    Core::IDocument *document() const override;
    QWidget *toolBar() override;

private:
    QVBoxLayout layout_;
    QComboBox scene_selector_;
    QTreeView scene_tree_view_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENETREEEDITOR_HPP
