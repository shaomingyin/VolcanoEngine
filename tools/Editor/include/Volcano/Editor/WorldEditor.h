//
//
#ifndef VOLCANO_EDITOR_WORLDEDITOR_H
#define VOLCANO_EDITOR_WORLDEDITOR_H

#include <memory>

#include <utils/id.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/editormanager/ieditorfactory.h>

#include <Volcano/Graphics/Renderer.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>
#include <Volcano/Editor/WorldDocument.h>
#include <Volcano/Editor/WorldEditorToolBar.h>
#include <Volcano/Editor/WorldEditorViewport.h>

VOLCANO_EDITOR_BEGIN

class WorldEditor: public Core::IEditor {
    Q_OBJECT

public:
    static const Utils::Id Id;

public:
    WorldEditor(Context& context);

public:
    Core::IDocument *document() const override;
    QWidget *toolBar() override;

private:
    Context& context_;
    std::shared_ptr<WorldDocument> document_;
    WorldEditorToolBar toolbar_;
    WorldEditorViewport viewport_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_WORLDEDITOR_H
