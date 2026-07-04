//
//
#ifndef VOLCANO_FILESYSTEM_H
#define VOLCANO_FILESYSTEM_H

#include <memory>
#include <filesystem>
#include <functional>

#include <SFML/System/InputStream.hpp>

#include <Volcano/Common.h>

#define VOLCANO_FILESYSTEM_BEGIN VOLCANO_BEGIN namespace FileSystem {
#define VOLCANO_FILESYSTEM_END } VOLCANO_END

VOLCANO_FILESYSTEM_BEGIN

using DirectoryCallback = std::function<bool(const std::filesystem::path&)>;

std::string toNativeString(const std::filesystem::path& p) noexcept;

void init(const char* argv0 = nullptr);
void shutdown();
void mount(const std::filesystem::path& path, const std::filesystem::path& os_path);
void unmount(const std::filesystem::path& path);
bool exists(const std::filesystem::path& path);
bool isFile(const std::filesystem::path& path);
bool isDirectory(const std::filesystem::path& path);
void remove(const std::filesystem::path& path);
void createDirectory(const std::filesystem::path& dirpath);
void listDirectory(const std::filesystem::path& dirpath, DirectoryCallback&& cb, bool recursive = false);
std::vector<std::filesystem::path> listDirectory(const std::filesystem::path& dirpath, bool recursive = false);
size_t fileSize(const std::filesystem::path& filepath);
std::unique_ptr<sf::InputStream> openFileForRead(const std::filesystem::path& filepath);
ByteArray readFile(const std::filesystem::path& filepath);

VOLCANO_FILESYSTEM_END

#endif // VOLCANO_FILESYSTEM_H
