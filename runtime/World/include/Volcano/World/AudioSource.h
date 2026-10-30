#ifndef VOLCANO_WORLD_AUDIOSOURCE_H
#define VOLCANO_WORLD_AUDIOSOURCE_H

#include <cmath>
#include <filesystem>
#include <stdexcept>
#include <utility>

#include <Volcano/Transform.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class AudioSource final {
public:
    enum class Playback {
        Stopped,
        Playing,
        Paused
    };

    AudioSource() = default;

    explicit AudioSource(std::filesystem::path source)
        : source_(std::move(source)) {
    }

    constexpr const std::filesystem::path& source() const noexcept {
        return source_;
    }

    void setSource(std::filesystem::path value) {
        source_ = std::move(value);
    }

    constexpr const Transform& offset() const noexcept {
        return offset_;
    }

    void setOffset(const Transform& value) noexcept {
        offset_ = value;
    }

    constexpr float gain() const noexcept {
        return gain_;
    }

    constexpr void setGain(float value) {
        if (!std::isfinite(value) || value < 0.0f) {
            throw std::invalid_argument("Audio source gain must be finite and nonnegative");
        }

        gain_ = value;
    }

    constexpr float pitch() const noexcept {
        return pitch_;
    }

    constexpr void setPitch(float value) {
        if (!std::isfinite(value) || value <= 0.0f) {
            throw std::invalid_argument("Audio source pitch must be finite and positive");
        }

        pitch_ = value;
    }

    constexpr bool isLooping() const noexcept {
        return looping_;
    }

    constexpr void setLooping(bool value) noexcept {
        looping_ = value;
    }

    constexpr bool isSpatial() const noexcept {
        return spatial_;
    }

    constexpr void setSpatial(bool value) noexcept {
        spatial_ = value;
    }

    constexpr Playback playback() const noexcept {
        return playback_;
    }

    constexpr void play() noexcept {
        playback_ = Playback::Playing;
    }

    constexpr void pause() noexcept {
        if (playback_ == Playback::Playing) {
            playback_ = Playback::Paused;
        }
    }

    constexpr void stop() noexcept {
        playback_ = Playback::Stopped;
    }

private:
    std::filesystem::path source_;
    Transform offset_;
    float gain_ = 1.0f;
    float pitch_ = 1.0f;
    bool looping_ = false;
    bool spatial_ = true;
    Playback playback_ = Playback::Stopped;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_AUDIOSOURCE_H
