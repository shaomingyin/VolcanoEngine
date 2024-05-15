//
//
#ifndef VOLCANO_EDITOR_SCENEVIEWEDITOR_HPP
#define VOLCANO_EDITOR_SCENEVIEWEDITOR_HPP

#include <coreplugin/editormanager/ieditor.h>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/SceneDocument.hpp>

VOLCANO_EDITOR_BEGIN

class SceneViewEditor: public Core::IEditor {
    Q_OBJECT

public:
    SceneViewEditor();

public:
    Core::IDocument *document() const override;
    QWidget *toolBar() override;

private:
    mutable SceneDocument document_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEVIEWEDITOR_HPP
