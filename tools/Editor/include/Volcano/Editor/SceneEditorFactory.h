//
//
#ifndef VOLCANO_EDITOR_SCENEEDITORFACTORY_H
#define VOLCANO_EDITOR_SCENEEDITORFACTORY_H

#include <coreplugin/editormanager/ieditorfactory.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>

VOLCANO_EDITOR_BEGIN

class SceneEditorFactory: public Core::IEditorFactory {
public:
    SceneEditorFactory(Context& context);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_SCENEEDITORFACTORY_H
