//
//
#ifndef VOLCANO_EDITOR_SCENETREEEDITORFACTORY_HPP
#define VOLCANO_EDITOR_SCENETREEEDITORFACTORY_HPP

#include <coreplugin/editormanager/ieditorfactory.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class SceneTreeEditorFactory: public Core::IEditorFactory {
public:
    SceneTreeEditorFactory();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENETREEEDITORFACTORY_HPP
