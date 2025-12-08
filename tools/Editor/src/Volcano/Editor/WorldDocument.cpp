//
//
#include <Volcano/Editor/WorldDocument.h>

VOLCANO_EDITOR_BEGIN

WorldDocument::WorldDocument(QObject* parent)
    : Core::IDocument(parent) {
    setMimeType(WORLD_FILE_MIMETYPE);
}

Utils::Result<> WorldDocument::open(const Utils::FilePath& filepath, const Utils::FilePath& real_fileath) {
    return Utils::ResultOk;
}

QByteArray WorldDocument::contents() const {
    return Core::IDocument::contents();
}

Utils::Result<> WorldDocument::setContents(const QByteArray &contents) {
    return Utils::ResultOk;
}

bool WorldDocument::isModified() const {
    return false;
}

bool WorldDocument::isSaveAsAllowed() const {
    return true;
}

Utils::Result<> WorldDocument::saveImpl(const Utils::FilePath& filepath, bool auto_save) {
    return Utils::ResultOk;
}

VOLCANO_EDITOR_END
