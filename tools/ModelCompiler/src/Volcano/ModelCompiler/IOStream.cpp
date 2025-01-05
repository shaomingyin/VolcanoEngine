//
//
#include <Volcano/ModelCompiler/IOStream.h>

VOLCANO_MODELCOMPILER_BEGIN

IOStream::IOStream(const char* path, const char* mode)
    : file_(path) {
    QFile::OpenMode open_mode = QFile::NotOpen;
    if (strchr(mode, 'r') != nullptr) {
        open_mode |= QFile::ReadOnly;
    }
    if (strchr(mode, 'w') != nullptr) {
        open_mode |= QFile::WriteOnly;
    }
    if (strchr(mode, 'a') != nullptr) {
        open_mode |= QFile::Append;
    }
    if (!file_.open(open_mode)) {
        throw std::runtime_error(std::format("Cannot open file {} in mode {}.", path, mode));
    }
}

size_t IOStream::Read(void* buf, size_t size, size_t count) {
    if (file_.openMode() & QIODevice::ReadOnly) {
        return file_.read((char*)buf, size * count);
    }
    return 0;
}

size_t IOStream::Write(const void* buf, size_t size, size_t count) {
    if (file_.openMode() & QIODevice::WriteOnly) {
        return file_.write((const char*)buf, size * count);
    }
    return 0;
}

aiReturn IOStream::Seek(size_t offset, aiOrigin origin) {
    if (file_.isSequential()) {
        return aiReturn_FAILURE;
    }

    bool ret;
    auto pos = file_.pos();

    switch (origin) {
    case aiOrigin_CUR:
        ret = file_.seek(pos + offset);
        break;
    case aiOrigin_SET:
        ret = file_.seek(offset);
        break;
    case aiOrigin_END:
        ret = file_.seek(file_.size() + offset);
        break;
    default:
        ret = false;
        break;
    }

    if (!ret) {
        return aiReturn_FAILURE;
    }

    return aiReturn_SUCCESS;
}

size_t IOStream::Tell() const {
    if (file_.isSequential()) {
        return 0;
    }
    return file_.pos();
}

size_t IOStream::FileSize() const {
    if (file_.isSequential()) {
        return 0;
    }
    return file_.size();
}

void IOStream::Flush() {
    // TODO
}

VOLCANO_MODELCOMPILER_END
