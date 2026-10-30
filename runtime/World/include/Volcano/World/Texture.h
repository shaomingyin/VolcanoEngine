#ifndef VOLCANO_WORLD_TEXTURE_H
#define VOLCANO_WORLD_TEXTURE_H

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <entt/entity/entity.hpp>

#include <Volcano/Size.h>
#include <Volcano/Gui/Document.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Texture {
public:
    enum class Source { File, Gui, Drawing, Camera };
    virtual ~Texture() = default;
    virtual Source source() const noexcept = 0;
};

class FileTexture final : public Texture {
public:
    explicit FileTexture(std::filesystem::path path) : path_(std::move(path)) {
    }
    Source source() const noexcept override {
        return Source::File;
    }
    const std::filesystem::path& path() const noexcept {
        return path_;
    }

private:
    std::filesystem::path path_;
};

class GeneratedTexture : public Texture {
public:
    Size size() const noexcept {
        return size_;
    }
    void setSize(Size value) {
        if (value.width <= 0 || value.height <= 0) {
            throw std::invalid_argument("Texture size must be positive");
        }
        size_ = value;
    }

protected:
    explicit GeneratedTexture(Size size) {
        setSize(size);
    }

private:
    Size size_;
};

class GuiTexture final : public GeneratedTexture {
public:
    GuiTexture(Size size, std::unique_ptr<Gui::Document> document)
        : GeneratedTexture(size), document_(std::move(document)) {
        if (!document_) {
            throw std::invalid_argument("GUI texture requires a document");
        }
    }
    Source source() const noexcept override {
        return Source::Gui;
    }
    Gui::Document& document() noexcept {
        return *document_;
    }
    const Gui::Document& document() const noexcept {
        return *document_;
    }

private:
    std::unique_ptr<Gui::Document> document_;
};

class DrawingTexture final : public GeneratedTexture {
public:
    DrawingTexture(Size size, std::string provider) : GeneratedTexture(size), provider_(std::move(provider)) {
    }
    Source source() const noexcept override {
        return Source::Drawing;
    }
    const std::string& provider() const noexcept {
        return provider_;
    }
    const std::vector<std::string>& dependencies() const noexcept {
        return dependencies_;
    }
    const std::vector<std::filesystem::path>& fonts() const noexcept {
        return fonts_;
    }
    void setFonts(std::vector<std::filesystem::path> values) {
        fonts_ = std::move(values);
    }
    void setDependencies(std::vector<std::string> values) {
        dependencies_ = std::move(values);
    }

private:
    std::string provider_;
    std::vector<std::string> dependencies_;
    std::vector<std::filesystem::path> fonts_;
};

class CameraTexture final : public GeneratedTexture {
public:
    CameraTexture(Size size, entt::entity camera) : GeneratedTexture(size), camera_(camera) {
    }
    Source source() const noexcept override {
        return Source::Camera;
    }
    entt::entity camera() const noexcept {
        return camera_;
    }
    void setCamera(entt::entity value) noexcept {
        camera_ = value;
    }

private:
    entt::entity camera_;
};

VOLCANO_WORLD_END

#endif
