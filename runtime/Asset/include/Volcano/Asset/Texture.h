#ifndef VOLCANO_ASSET_TEXTURE_H
#define VOLCANO_ASSET_TEXTURE_H

#include <cstddef>
#include <cstdint>

#include <google/protobuf/io/zero_copy_stream.h>

#include <Volcano/Asset/Common.h>

VOLCANO_ASSET_BEGIN

// Single-level RGBA8, bottom row first, with premultiplied alpha.
// Derives from ZeroCopyInputStream to stream the raw pixel payload.
class Texture final : public google::protobuf::io::ZeroCopyInputStream {
public:
    explicit Texture(google::protobuf::io::ZeroCopyInputStream& source);

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&&) noexcept = default;
    Texture& operator=(Texture&&) noexcept = default;
    ~Texture() override = default;

    const TextureHeader& header() const noexcept {
        return header_;
    }

    std::uint32_t width() const noexcept {
        return header_.width();
    }

    std::uint32_t height() const noexcept {
        return header_.height();
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
    TextureHeader header_;
    std::size_t payload_size_ = 0;
    std::size_t bytes_read_ = 0;
};

VOLCANO_ASSET_END

#endif // VOLCANO_ASSET_TEXTURE_H
