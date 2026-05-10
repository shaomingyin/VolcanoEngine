//
//
#ifndef VOLCANO_FILESYSTEM_H
#define VOLCANO_FILESYSTEM_H

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include <async++.h>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class File {
public:
	File() = default;
	virtual ~File() = default;

public:
	virtual size_t size() const = 0;
	virtual ByteArray read() const = 0;
	virtual void write(const ByteArray& data) = 0;
};

struct Stat {
	enum class Type {
		File,
		Directory,
	} type;

	std::string name;
	Type type;
	size_t size;
	std::chrono::system_clock::time_point last_modified_time;
};

class FileSystem {
public:
	FileSystem() = default;
	virtual ~FileSystem() = default;

public:
	virtual Stat stat(const std::string& path) const = 0;
	virtual void remove(const std::string& path) = 0;
	virtual std::vector<std::string> enumDirectory(const std::string& dirpath) const = 0;
	virtual void createDirectory(const std::string& dirpath) = 0;
	virtual size_t fileSize(const std::string& filepath) const = 0;
	virtual ByteArray readFile(const std::string& filepath) const = 0;
	virtual void writeFile(const std::string& filepath, const ByteArray& data) = 0;
	virtual std::unique_ptr<File> openRead(const std::string& filepath) = 0;
	virtual std::unique_ptr<File> openWrite(const std::string& filepath, bool create) = 0;
};

extern FileSystem nativeFileSystem;

void initFileSystem(std::shared_ptr<FileSystem> fs);
void shutdownFileSystem();
Stat stat(const std::string& path);
bool exists(const std::string& path);
void remove(const std::string& path);
std::vector<std::string> enumDirectory(const std::string& dirpath);
void createDirectory(const std::string& dirpath);
size_t fileSize(const std::string& filepath);
ByteArray readFile(const std::string& filepath);
void writeFile(const std::string& filepath, const ByteArray& data);
std::unique_ptr<File> openRead(const std::string& filepath);
std::unique_ptr<File> openWrite(const std::string& filepath, bool create = true);

template <typename SCHEDULER>
auto asyncReadFile(SCHEDULER& scheduler, const std::string& filepath) {
	return async::spawn(scheduler, [filepath]() {
		return readFile(filepath);
	});
}

template <typename SCHEDULER>
auto asyncReadFile(const std::string& filepath) {
	return asyncReadFile(async::default_scheduler(), filepath);
}

VOLCANO_END

#endif // VOLCANO_FILESYSTEM_H
