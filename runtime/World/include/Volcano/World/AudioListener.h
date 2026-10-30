#ifndef VOLCANO_WORLD_AUDIOLISTENER_H
#define VOLCANO_WORLD_AUDIOLISTENER_H

#include <cmath>
#include <stdexcept>

#include <Volcano/Transform.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class AudioListener final {
public:
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
            throw std::invalid_argument("Audio listener gain must be finite and nonnegative");
        }

        gain_ = value;
    }

private:
    Transform offset_;
    float gain_ = 1.0f;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_AUDIOLISTENER_H
