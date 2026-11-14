#ifndef VOLCANO_WORLD_TEXTURE_H
#define VOLCANO_WORLD_TEXTURE_H

#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <utility>

#include <Volcano/Size.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Texture {
public:
    enum class Kind : std::uint8_t {
        Path,
        Canvas,
        Document,
        Graphics
    };

public:
    explicit Texture(std::filesystem::path source)
        : source_(std::move(source)) {
        if (source_.empty()) {
            throw std::invalid_argument("Texture source path cannot be empty");
        }
    }

    virtual ~Texture() = default;

    Kind kind() const noexcept {
        return kind_;
    }

    bool isCanvas() const noexcept {
        return kind_ == Kind::Canvas || kind_ == Kind::Document;
    }

    const std::filesystem::path& source() const noexcept {
        return source_;
    }

    virtual Size size() const noexcept {
        return {};
    }

protected:
    explicit Texture(Kind kind) noexcept
        : kind_(kind) {}

    Texture(const Texture&) = default;
    Texture& operator=(const Texture&) = default;
    Texture(Texture&&) noexcept = default;
    Texture& operator=(Texture&&) noexcept = default;

private:
    Kind kind_ = Kind::Path;
    std::filesystem::path source_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_TEXTURE_H
