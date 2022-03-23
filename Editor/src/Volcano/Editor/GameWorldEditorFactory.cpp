//
//
#include <memory>

#include <Volcano/Editor/GameWorldEditor.hpp>
#include <Volcano/Editor/GameWorldEditorFactory.hpp>

VOLCANO_EDITOR_BEGIN

GameWorldEditorFactory::GameWorldEditorFactory(void)
{
    addMimeType("vew");
    setDisplayName("Volcano Engine World");

    setEditorCreator([] () -> Core::IEditor * {
        auto editor = std::make_unique<GameWorldEditor>();
        if (!editor)
            return nullptr;
        return editor.release();
    });
}

GameWorldEditorFactory::~GameWorldEditorFactory(void)
{
}

VOLCANO_EDITOR_END
