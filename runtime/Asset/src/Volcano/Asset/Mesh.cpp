#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>
#include <string>

#include "Schema.pb.h"
#include "Format.h"

#include <Volcano/Asset/Mesh.h>

VOLCANO_ASSET_BEGIN

Mesh::Mesh(google::protobuf::io::ZeroCopyInputStream& source)
    : source_(&source) {
    initializeHeader();
}

bool Mesh::Next(const void** data, int* size) {
    if (source_ == nullptr || bytes_read_ >= payload_size_) {
        return false;
    }

    const void* chunk = nullptr;
    int chunkSize = 0;
    if (!source_->Next(&chunk, &chunkSize)) {
        return false;
    }

    if (chunkSize <= 0) {
        *data = chunk;
        *size = 0;
        return true;
    }

    const std::size_t remaining = payload_size_ - bytes_read_;
    if (static_cast<std::size_t>(chunkSize) > remaining) {
        const int excess = chunkSize - static_cast<int>(remaining);
        source_->BackUp(excess);
        *data = chunk;
        *size = static_cast<int>(remaining);
        bytes_read_ += remaining;
    } else {
        *data = chunk;
        *size = chunkSize;
        bytes_read_ += static_cast<std::size_t>(chunkSize);
    }
    return true;
}

void Mesh::BackUp(int count) {
    if (count <= 0 || source_ == nullptr) {
        return;
    }

    const std::size_t backupCount = std::min(static_cast<std::size_t>(count), bytes_read_);
    source_->BackUp(static_cast<int>(backupCount));
    bytes_read_ -= backupCount;
}

bool Mesh::Skip(int count) {
    if (count <= 0) {
        return true;
    }
    if (source_ == nullptr) {
        return false;
    }

    const std::size_t remaining = payload_size_ - bytes_read_;
    if (static_cast<std::size_t>(count) > remaining) {
        return false;
    }

    if (!source_->Skip(count)) {
        return false;
    }

    bytes_read_ += static_cast<std::size_t>(count);
    return true;
}

int64_t Mesh::ByteCount() const {
    return static_cast<int64_t>(bytes_read_);
}

void Mesh::initializeHeader() {
    uint32_t magic = 0;
    if (!Format::readExactBytes(*source_, &magic, sizeof(magic)) || magic != 0x484D4556) {
        throw std::invalid_argument("Invalid mesh resource magic");
    }

    uint32_t headerSize = 0;
    if (!Format::readExactBytes(*source_, &headerSize, sizeof(headerSize)) || headerSize == 0) {
        throw std::invalid_argument("Invalid mesh header size");
    }

    std::string headerBytes(headerSize, '\0');
    if (!Format::readExactBytes(*source_, headerBytes.data(), static_cast<int>(headerSize))) {
        throw std::invalid_argument("Truncated mesh header data");
    }

    if (!header_.ParseFromString(headerBytes)) {
        throw std::invalid_argument("Failed to parse mesh header protobuf");
    }

    if (header_.index_count() % 3 != 0) {
        throw std::invalid_argument("Mesh index count must be a multiple of 3");
    }

    payload_size_ = Format::checkedSum(Format::checkedProduct(header_.vertex_count(), sizeof(Vertex)),
                                        Format::checkedProduct(header_.index_count(), sizeof(uint32_t)));
}

VOLCANO_ASSET_END
