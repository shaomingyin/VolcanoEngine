//
//
#ifndef VOLCANO_GRAPHICS_IOSYSTEM_H
#define VOLCANO_GRAPHICS_IOSYSTEM_H

#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class IOSystem: public Assimp::IOSystem {
public:
    IOSystem() = default;
    ~IOSystem() override = default;

public:
    bool Exists(const char* path) const override;
    char getOsSeparator() const override;
    Assimp::IOStream* Open(const char* path, const char* mode) override;
    void Close(Assimp::IOStream* file) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_IOSYSTEM_H
