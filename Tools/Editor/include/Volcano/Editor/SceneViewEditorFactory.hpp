//
//
#ifndef VOLCANO_EDITOR_SCENEVIEWEDITORFACTORY_HPP
#define VOLCANO_EDITOR_SCENEVIEWEDITORFACTORY_HPP

#include <coreplugin/editormanager/ieditorfactory.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class SceneViewEditorFactory: public Core::IEditorFactory {
public:
    SceneViewEditorFactory();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEVIEWEDITORFACTORY_HPP
