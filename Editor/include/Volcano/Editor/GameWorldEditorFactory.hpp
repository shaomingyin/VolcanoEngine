//
//
#ifndef VOLCANO_EDITOR_GAMEWORLDEDITORFACTORY_HPP
#define VOLCANO_EDITOR_GAMEWORLDEDITORFACTORY_HPP

#include <coreplugin/idocument.h>
#include <coreplugin/editormanager/ieditorfactory.h>

#include <Volcano/Game/Object.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class GameWorldEditorFactory: public Core::IEditorFactory {
    Q_OBJECT

public:
    GameWorldEditorFactory(void);
    ~GameWorldEditorFactory(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_GAMEWORLDEDITORFACTORY_HPP
