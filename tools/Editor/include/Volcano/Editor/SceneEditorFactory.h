//
//
#ifndef VOLCANO_EDITOR_SCENEEDITORFACTORY_H
#define VOLCANO_EDITOR_SCENEEDITORFACTORY_H

#include <coreplugin/editormanager/ieditorfactory.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class SceneEditorFactory: public Core::IEditorFactory {
public:
    SceneEditorFactory();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEEDITORFACTORY_H
