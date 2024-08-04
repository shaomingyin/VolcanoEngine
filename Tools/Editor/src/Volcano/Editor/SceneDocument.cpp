//
//
#include <Volcano/Editor/SceneDocument.hpp>

VOLCANO_EDITOR_BEGIN

const QString SceneDocument::MimeType(VOLCANO_EDITOR_MIME_SCENE_FILE);

SceneDocument::SceneDocument() {
    setMimeType(MimeType);
};

VOLCANO_EDITOR_END
