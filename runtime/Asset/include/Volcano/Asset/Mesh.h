#ifndef VOLCANO_ASSET_MESH_H
#define VOLCANO_ASSET_MESH_H

#include <array>
#include <cstddef>
#include <cstdint>

#include <google/protobuf/io/zero_copy_stream.h>

#include <Volcano/Asset/Common.h>

VOLCANO_ASSET_BEGIN

// Derives from ZeroCopyInputStream to stream vertices followed by indices.
class Mesh final : public google::protobuf::io::ZeroCopyInputStream {
public:
    struct Vertex final {
        std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
        std::array<float, 3> normal = {0.0f, 0.0f, 1.0f};
        std::array<float, 2> texcoord = {0.0f, 0.0f};
    };

public:
    explicit Mesh(google::protobuf::io::ZeroCopyInputStream& source);

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) noexcept = default;
    Mesh& operator=(Mesh&&) noexcept = default;
    ~Mesh() override = default;

    const MeshHeader& header() const noexcept {
        return header_;
    }

    std::uint32_t vertexCount() const noexcept {
        return header_.vertex_count();
    }

    std::uint32_t indexCount() const noexcept {
        return header_.index_count();
    }

    std::size_t vertexDataSize() const noexcept {
        return static_cast<std::size_t>(header_.vertex_count()) * sizeof(Vertex);
    }

    std::size_t indexDataSize() const noexcept {
        return static_cast<std::size_t>(header_.index_count()) * sizeof(std::uint32_t);
    }

    std::size_t payloadSize() const noexcept {
        return payload_size_;
    }

    bool Next(const void** data, int* size) override;
    void BackUp(int count) override;
    bool Skip(int count) override;
    int64_t ByteCount() const override;

private:
    void initializeHeader();

private:
    google::protobuf::io::ZeroCopyInputStream* source_ = nullptr;
    MeshHeader header_;
    std::size_t payload_size_ = 0;
    std::size_t bytes_read_ = 0;
};

VOLCANO_ASSET_END

#endif // VOLCANO_ASSET_MESH_H
