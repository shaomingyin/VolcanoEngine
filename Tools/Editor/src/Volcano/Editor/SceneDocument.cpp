//
//
#include <Volcano/Editor/SceneDocument.hpp>

VOLCANO_EDITOR_BEGIN

const QString SceneDocument::MimeType("application/volcano-scene");

SceneDocument::SceneDocument() {
    setMimeType(MimeType);
};

VOLCANO_EDITOR_END
