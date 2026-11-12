#ifndef VOLCANO_FRAMEWORK_DRAWING_H
#define VOLCANO_FRAMEWORK_DRAWING_H

#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <Volcano/Size.h>
#include <Volcano/Framework/Common.h>

VOLCANO_FRAMEWORK_BEGIN

class Drawing final {
public:
    Drawing(std::string name, Size size, std::string provider)
        : name_(std::move(name)), size_(size), provider_(std::move(provider)) {
        if (name_.empty()) {
            throw std::invalid_argument("Drawing name cannot be empty");
        }
        if (size_.width <= 0 || size_.height <= 0) {
            throw std::invalid_argument("Drawing size must be positive");
        }
    }

    const std::string& name() const noexcept {
        return name_;
    }

    void setName(std::string value) {
        if (value.empty()) {
            throw std::invalid_argument("Drawing name cannot be empty");
        }
        name_ = std::move(value);
    }

    Size size() const noexcept {
        return size_;
    }

    void setSize(Size value) {
        if (value.width <= 0 || value.height <= 0) {
            throw std::invalid_argument("Drawing size must be positive");
        }
        size_ = value;
    }

    const std::string& provider() const noexcept {
        return provider_;
    }

    void setProvider(std::string value) {
        provider_ = std::move(value);
    }

    const std::vector<std::string>& dependencies() const noexcept {
        return dependencies_;
    }

    void setDependencies(std::vector<std::string> values) {
        dependencies_ = std::move(values);
    }

    const std::vector<std::filesystem::path>& fonts() const noexcept {
        return fonts_;
    }

    void setFonts(std::vector<std::filesystem::path> values) {
        fonts_ = std::move(values);
    }

private:
    std::string name_;
    Size size_;
    std::string provider_;
    std::vector<std::string> dependencies_;
    std::vector<std::filesystem::path> fonts_;
};

VOLCANO_FRAMEWORK_END

#endif // VOLCANO_FRAMEWORK_DRAWING_H
