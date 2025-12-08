//
//
#ifndef VOLCANO_EDITOR_WORLDEDITORFACTORY_H
#define VOLCANO_EDITOR_WORLDEDITORFACTORY_H

#include <coreplugin/editormanager/ieditorfactory.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>

VOLCANO_EDITOR_BEGIN

class WorldEditorFactory: public Core::IEditorFactory {
public:
    WorldEditorFactory(Context& context);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_WORLDEDITORFACTORY_H
