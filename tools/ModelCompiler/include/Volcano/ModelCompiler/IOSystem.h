//
//
#ifndef VOLCANO_MODELCOMPILER_IOSYSTEM_H
#define VOLCANO_MODELCOMPILER_IOSYSTEM_H

#include <assimp/IOSystem.hpp>

#include <Volcano/ModelCompiler/Common.h>

VOLCANO_MODELCOMPILER_BEGIN

class IOSystem: public Assimp::IOSystem {
public:
    IOSystem();

public:
    bool Exists(const char* path) const override;
    char getOsSeparator() const override;
    Assimp::IOStream* Open(const char* path, const char* mode) override;
    void Close(Assimp::IOStream* io) override;
};

VOLCANO_MODELCOMPILER_END

#endif // VOLCANO_MODELCOMPILER_IOSYSTEM_H
