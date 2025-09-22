//
//
#include <QFile>
#include <QFileInfo>

#include <Volcano/ModelCompiler/IOStream.h>
#include <Volcano/ModelCompiler/IOSystem.h>

VOLCANO_MODELCOMPILER_BEGIN

IOSystem::IOSystem() {
}

bool IOSystem::Exists(const char* path) const {
    QFileInfo fi(path);
    return fi.exists();
}

char IOSystem::getOsSeparator() const {
    return '/';
}

Assimp::IOStream* IOSystem::Open(const char* path, const char* mode = "rb") {
    return new IOStream(path, mode);
}

void IOSystem::Close(Assimp::IOStream* io) {
    delete io;
}

VOLCANO_MODELCOMPILER_END
