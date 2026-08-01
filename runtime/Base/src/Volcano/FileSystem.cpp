//
//
#include <map>
#include <mutex>

#ifdef _WIN32
#	include <Windows.h>
#endif

#include <physfs.h>

#include <Volcano/FileSystem.h>

VOLCANO_FILESYSTEM_BEGIN

static void throwError(std::string&& text) {
	auto errmsg = text + PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode());
	spdlog::error("Filesystem exception: {}", errmsg);
	throw std::runtime_error(errmsg);
}

static std::string toNativeString(const std::filesystem::path& p) noexcept {
#if defined(_WIN32)
	std::wstring w = p.native();

	if (w.empty()) {
		return {};
	}

	int size = WideCharToMultiByte(
		CP_UTF8, 0,
		w.data(), (int)w.size(),
		nullptr, 0,
		nullptr, nullptr
	);

	std::string result(size, 0);

	WideCharToMultiByte(
		CP_UTF8, 0,
		w.data(), (int)w.size(),
		result.data(), size,
		nullptr, nullptr
	);

	return result;
#else
	return p.native();
#endif
}

class PHYSFS_FileInputStream final: public sf::InputStream {
public:
	PHYSFS_FileInputStream(PHYSFS_File* fp, bool owned = true)
		: fp_(fp)
		, owned_(owned) {
		VOLCANO_ASSERT(fp_ != nullptr);
	}

	~PHYSFS_FileInputStream() {
		if (owned_) {
			PHYSFS_close(fp_);
		}
	}

public:
	std::optional<std::size_t> read(void* data, std::size_t size) override {
		auto ret = PHYSFS_readBytes(fp_, data, size);
		if (ret < 0) {
			return std::nullopt;
		}
		return ret;
	}

	std::optional<std::size_t> seek(std::size_t position) override {
		auto ret = PHYSFS_seek(fp_, position);
		if (!ret) {
			return std::nullopt;
		}
		return ret;
	}

	std::optional<std::size_t> tell() override {
		auto ret = PHYSFS_tell(fp_);
		if (ret < 0) {
			return std::nullopt;
		}
		return ret;
	}

	std::optional<std::size_t> getSize() override {
		auto ret = PHYSFS_fileLength(fp_);
		if (ret < 0) {
			return std::nullopt;
		}
		return ret;
	}

private:
	PHYSFS_File* fp_;
	bool owned_;
};

void init(const char* argv0) {
	spdlog::info("Initializing filesystem...");
	if (PHYSFS_isInit()) {
		throw std::runtime_error("Filesystem is already initialized");
	}
	int ret = PHYSFS_init(argv0);
	if (!ret) {
		throwError("Failed to initialize filesystem: ");
	}
}

void shutdown() {
	spdlog::info("Shutting down filesystem...");
	if (PHYSFS_isInit()) {
		if (!PHYSFS_deinit()) {
			throwError("Failed to deinitialize filesystem: ");
		}
	}
}

void mount(const std::filesystem::path& path, const std::filesystem::path& os_path) {
	VOLCANO_ASSERT(!path.empty());
	VOLCANO_ASSERT(!os_path.empty());
	int ret = PHYSFS_mount(os_path.generic_string().c_str(), path.generic_string().c_str(), 0);
	if (!ret) {
		throwError("Failed to mount path '" + os_path.string() + "' at '" + path.string() + "': ");
	}
}

void unmount(const std::filesystem::path& path) {
	VOLCANO_ASSERT(!path.empty());
	int ret = PHYSFS_unmount(path.string().c_str());
	if (!ret) {
		throwError("Failed to unmount path '" + path.string() + "': ");
	}
}

bool exists(const std::filesystem::path& path) {
	VOLCANO_ASSERT(!path.empty());
	return PHYSFS_exists(path.string().c_str()) != 0;
}

bool isFile(const std::filesystem::path& path) {
	VOLCANO_ASSERT(!path.empty());
	PHYSFS_Stat st;
	int ret = PHYSFS_stat(path.string().c_str(), &st);
	if (!ret) {
		throwError("Failed to stat path '" + path.string() + "': ");
	}
	return st.filetype == PHYSFS_FILETYPE_REGULAR;
}

bool isDirectory(const std::filesystem::path& path) {
	VOLCANO_ASSERT(!path.empty());
	PHYSFS_Stat st;
	int ret = PHYSFS_stat(path.string().c_str(), &st);
	if (!ret) {
		throwError("Failed to stat path '" + path.string() + "': ");
	}
	return st.filetype == PHYSFS_FILETYPE_DIRECTORY;
}

void remove(const std::filesystem::path& path) {
	VOLCANO_ASSERT(!path.empty());
	int ret = PHYSFS_delete(path.string().c_str());
	if (!ret) {
		throwError("Failed to remove path '" + path.string() + "': ");
	}
}

void createDirectory(const std::filesystem::path& dirpath) {
	VOLCANO_ASSERT(!dirpath.empty());
	int ret = PHYSFS_mkdir(dirpath.string().c_str());
	if (!ret) {
		throwError("Failed to create directory '" + dirpath.string() + "': ");
	}
}

void listDirectory(const std::filesystem::path& dirpath, DirectoryCallback&& cb, bool recursive) {
	VOLCANO_ASSERT(!dirpath.empty());
	VOLCANO_ASSERT(!cb);
	auto ret = PHYSFS_enumerateFiles(dirpath.string().c_str());
	for (const char* p = *ret; p != nullptr; ++p) {
		if (!cb(p)) {
			break;
		}
	}
}

std::vector<std::filesystem::path> listDirectory(const std::filesystem::path& dirpath, bool recursive) {
	VOLCANO_ASSERT(!dirpath.empty());
	std::vector<std::filesystem::path> result;
	listDirectory(dirpath, [&result](const std::filesystem::path& p) {
		result.push_back(p);
		return true;
	}, recursive);
    return result;
}

size_t fileSize(const std::filesystem::path& filepath) {
	VOLCANO_ASSERT(!filepath.empty());
	PHYSFS_Stat st;
	int ret = PHYSFS_stat(filepath.string().c_str(), &st);
	if (!ret) {
		throwError("Failed to stat file '" + filepath.string() + "': ");
	}
	return st.filesize;
}

std::unique_ptr<sf::InputStream> openFileForRead(const std::filesystem::path& filepath) {
	VOLCANO_ASSERT(!filepath.empty());
	auto fp = PHYSFS_openRead(filepath.generic_string().c_str());
	if (fp == nullptr) {
		throwError(std::format("Failed to read physfs file: {}", filepath.generic_string()));
	}
	return std::make_unique<PHYSFS_FileInputStream>(fp);
}

ByteArray readFile(sf::InputStream& input) {
	ByteArray data(input.getSize().value_or(0));
	auto read_ret = input.read(data.data(), data.size());
	if (!read_ret.has_value() || read_ret.value() != data.size()) {
		throw std::runtime_error("Failed to read file.");
	}
	return data;
}

ByteArray readFile(const std::filesystem::path& filepath) {
	VOLCANO_ASSERT(!filepath.empty());
	return readFile(*openFileForRead(filepath));
}

VOLCANO_FILESYSTEM_END
