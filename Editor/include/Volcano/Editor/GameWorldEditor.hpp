//
//
#ifndef VOLCANO_EDITOR_GAMEWORLDEDITOR_HPP
#define VOLCANO_EDITOR_GAMEWORLDEDITOR_HPP

#include <coreplugin/idocument.h>
#include <coreplugin/editormanager/ieditor.h>

#include <Volcano/Game/Object.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class GameWorldEditor: public Core::IEditor {
    Q_OBJECT

public:
    GameWorldEditor(void);
    ~GameWorldEditor(void) override;

public:
    Core::IDocument *document(void) const override;
    QWidget *toolBar(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_GAMEWORLDEDITOR_HPP
