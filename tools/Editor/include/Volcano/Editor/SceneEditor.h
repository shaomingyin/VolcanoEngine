//
//
#ifndef VOLCANO_EDITOR_SCENEEDITOR_H
#define VOLCANO_EDITOR_SCENEEDITOR_H

#include <memory>

#include <utils/id.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/ieditorfactory.h>

#include <Volcano/Graphics/Renderer.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>
#include <Volcano/Editor/SceneDocument.h>
#include <Volcano/Editor/SceneEditorToolBar.h>
#include <Volcano/Editor/SceneEditorViewport.h>

VOLCANO_EDITOR_BEGIN

class SceneEditor: public Core::IEditor {
    Q_OBJECT

public:
    static const Utils::Id Id;

public:
    SceneEditor(Context& context);

public:
    Core::IDocument *document() const override;
    QWidget *toolBar() override;

private:
    Context& context_;
    std::shared_ptr<SceneDocument> document_;
    SceneEditorToolBar toolbar_;
    SceneEditorViewport viewport_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEEDITOR_H
