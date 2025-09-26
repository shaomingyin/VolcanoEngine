//
//
#include <Volcano/Editor/SceneDocument.h>

VOLCANO_EDITOR_BEGIN

SceneDocument::SceneDocument(QObject* parent)
    : Core::IDocument(parent) {
    setMimeType(SCENE_FILE_MIMETYPE);
}

Utils::Result<> SceneDocument::open(const Utils::FilePath& filepath, const Utils::FilePath& real_fileath) {
    return Utils::ResultOk;
}

QByteArray SceneDocument::contents() const {
    return Core::IDocument::contents();
}

Utils::Result<> SceneDocument::setContents(const QByteArray &contents) {
    return Utils::ResultOk;
}

bool SceneDocument::isModified() const {
    return false;
}

bool SceneDocument::isSaveAsAllowed() const {
    return true;
}

Utils::Result<> SceneDocument::saveImpl(const Utils::FilePath& filepath, bool auto_save) {
    return Utils::ResultOk;
}

VOLCANO_EDITOR_END
