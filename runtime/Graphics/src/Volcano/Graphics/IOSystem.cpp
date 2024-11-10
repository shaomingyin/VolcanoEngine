//
//
#include <Volcano/Error.h>
#include <Volcano/Graphics/IOSystem.h>

VOLCANO_GRAPHICS_BEGIN

class PhysFile: public Assimp::IOStream {
public:
    PhysFile(const char* path, const char* mode)
        : fp_(openFile(path, mode)) {
        if (fp_ == nullptr) {
            throw Error(Errc::InvalidParameter);
        }
    }

    ~PhysFile() {
        PHYSFS_close(fp_);
    }

public:
    size_t Read(void* buf, size_t size, size_t count) {
        return PHYSFS_readBytes(fp_, buf, size * count);
    }

    size_t Write(const void* buf, size_t size, size_t count) {
        return PHYSFS_writeBytes(fp_, buf, size * count);
    }

    aiReturn Seek(size_t offset, aiOrigin origin) {
        auto pos = PHYSFS_tell(fp_);

        switch (origin) {
        case aiOrigin_SET:
            pos = offset;
            break;
        case aiOrigin_CUR:
            pos += offset;
            break;
        case aiOrigin_END:
            pos = PHYSFS_fileLength(fp_) + offset;
            break;
        }

        auto ret = PHYSFS_seek(fp_, pos);
        if (ret < 0) {
            aiReturn_FAILURE;
        }

        return aiReturn_SUCCESS;
    }

    size_t Tell() const {
        return PHYSFS_tell(fp_);
    }

    size_t FileSize() const {
        return PHYSFS_fileLength(fp_);
    }

    void Flush() {
        PHYSFS_flush(fp_);
    }

private:
    static PHYSFS_File* openFile(const char* path, const char* mode) {
        if (strcmp(mode, "r") == 0 || strcmp(mode, "rb") == 0) {
            return PHYSFS_openRead(path);
        }
        if (strcmp(mode, "w") == 0 || strcmp(mode, "wb") == 0) {
            return PHYSFS_openWrite(path);
        }
        if (strcmp(mode, "a") == 0) {
            return PHYSFS_openAppend(path);
        }
        return nullptr;
    }

private:
    PHYSFS_File* fp_;
};

bool IOSystem::Exists(const char* path) const {
    return PHYSFS_exists(path);
}

char IOSystem::getOsSeparator() const {
    return '/';
}

Assimp::IOStream* IOSystem::Open(const char* path, const char* mode) {
    return new PhysFile(path, mode);
}

void IOSystem::Close(Assimp::IOStream* file) {
    delete file;
}

VOLCANO_GRAPHICS_END
