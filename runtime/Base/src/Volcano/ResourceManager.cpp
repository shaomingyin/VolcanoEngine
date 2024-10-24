//
//
#include <Volcano/Error.h>
#include <Volcano/ResourceManager.h>

VOLCANO_BEGIN

// NativeFile

class NativeFile: public Resource {
public:
	NativeFile(const std::filesystem::path& path, int mode)
		: fp_(SDL_RWFromFile(path.u8string().c_str(), "r")) {
		if (fp_ == nullptr) {
			throw Error(Errc::Unknown);
		}
	}

public:
	int64_t size() const override {
		return SDL_RWsize(fp_);
	}

	bool load(void* buf, int64_t buf_size) override {
		if (SDL_RWseek(fp_, 0, RW_SEEK_SET) < 0) {
			return false;
		}
		auto read_size = SDL_RWread(fp_, buf, buf_size, 1);
		return (read_size == buf_size);
	}

private:
	SDL_RWops* fp_;
};

// ZipFile

class ZipFile : public Resource {
public:
	ZipFile(std::shared_ptr<mz_zip_archive> zip_archive, const std::filesystem::path& path)
		: zip_archive_(zip_archive)
		, index_(mz_zip_reader_locate_file(zip_archive.get(), path.u8string().c_str(), nullptr, MZ_ZIP_FLAG_CASE_SENSITIVE)) {
		if (index_ < 0) {
			throw Error(Errc::NotExisted);
		}
		if (!mz_zip_reader_file_stat(zip_archive.get(), index_, &stat_)) {
			throw Error(Errc::InvalidContext);
		}
		if (stat_.m_is_directory) {
			throw Error(Errc::NotExisted);
		}
	}

public:
	int64_t size() const override {
		return stat_.m_uncomp_size;
	}

	bool load(void* buf, int64_t buf_size) override {
		if (buf_size < stat_.m_uncomp_size) {
			return false;
		}
		struct Args {
			void* buf;
			int64_t buf_size;
		} args = { buf, buf_size };
		auto cb = [](void* data, mz_uint64 offset, const void* buf, size_t n) -> size_t {
			Args* args = reinterpret_cast<Args*>(data);
			if (offset >= args->buf_size) {
				return 0;
			}
			auto left_size = args->buf_size - offset;
			auto copy_size = std::min(left_size, n);
			memcpy(args->buf, buf, copy_size);
			return copy_size;
		};
		return mz_zip_reader_extract_to_callback(zip_archive_.get(), index_, cb, &args, 0);
	}

private:
	std::shared_ptr<mz_zip_archive> zip_archive_;
	mz_zip_archive_file_stat stat_;
	int index_;
};

// ResourceManager::MountInfo

ResourceManager::MountInfo::MountInfo(const std::filesystem::path& path, const std::filesystem::path& native_path)
	: path_(std::filesystem::canonical(path))
	, native_path_(std::filesystem::canonical(native_path)) {
	if (std::filesystem::is_directory(native_path)) {
		native_.emplace<std::filesystem::path>(native_path);
	} else if (std::filesystem::is_regular_file(native_path) && native_path.extension() == ".zip") {
		auto zip_archive = std::make_shared<mz_zip_archive>();
		mz_zip_zero_struct(zip_archive.get());
		if (!mz_zip_reader_init_file(zip_archive.get(), native_path.u8string().c_str(), 0)) {
			throw Error(Errc::InvalidParameter);
		}
		native_.emplace<std::shared_ptr<mz_zip_archive>>(zip_archive);
	} else {
        throw Error(Errc::NotSupported);
    }
}

ResourceManager::MountInfo::~MountInfo() {
	if (std::holds_alternative<std::shared_ptr<mz_zip_archive>>(native_)) {
		auto p = std::get<std::shared_ptr<mz_zip_archive>>(native_);
		mz_zip_reader_end(p.get());
	}
}

bool ResourceManager::MountInfo::exists(const std::filesystem::path& path) const {
	bool ret;
	if (std::holds_alternative<std::shared_ptr<mz_zip_archive>>(native_)) {
		auto p = std::get<std::shared_ptr<mz_zip_archive>>(native_);
		int index = mz_zip_reader_locate_file(p.get(), path.u8string().c_str(), nullptr, MZ_ZIP_FLAG_CASE_SENSITIVE);
		ret = (index >= 0);
	} else {
		auto native_path = std::get<std::filesystem::path>(native_);
		ret = std::filesystem::exists(native_path/path);
	}
	return ret;
}

void ResourceManager::MountInfo::enumDir(const std::filesystem::path& path, DirEntrySet& output) const {
	if (std::holds_alternative<std::shared_ptr<mz_zip_archive>>(native_)) {
		enumZipDir(path, output);
	} else {
		enumNativeDir(path, output);
	}
}

std::shared_ptr<Resource> ResourceManager::MountInfo::get(const std::filesystem::path& path) {
	return nullptr;
}

void ResourceManager::MountInfo::enumNativeDir(const std::filesystem::path& path, DirEntrySet& output) const {
	auto native_path = std::get<std::filesystem::path>(native_);
	for (const auto& dent : std::filesystem::directory_iterator(native_path / path)) {
		if (dent.is_directory()) {
			output.emplace(DirEntry::Type::Directory, dent.path().filename().string());
		} else if (dent.is_regular_file()) {
			output.emplace(DirEntry::Type::File, dent.path().filename().string());
		} else {
			output.emplace(DirEntry::Type::Unknown, dent.path().filename().string());
		}
	}
}

void ResourceManager::MountInfo::enumZipDir(const std::filesystem::path& path, DirEntrySet& output) const {
	auto real_path = std::filesystem::relative(path, "/");
	auto p = std::get<std::shared_ptr<mz_zip_archive>>(native_);
	auto index = mz_zip_reader_locate_file(p.get(), real_path.u8string().c_str(), nullptr, MZ_ZIP_FLAG_CASE_SENSITIVE);
	if (index < 0) {
		return;
	}
	mz_zip_archive_file_stat st;
	mz_bool bret = mz_zip_reader_file_stat(p.get(), index, &st);
	if (!bret || !st.m_is_directory) {
		return;
	}
#if 0
	for (mz_uint i = 0; i < n; ++i) {
		char filename[512] = { 0 };
		mz_uint filename_len = mz_zip_reader_get_filename(p.get(), i, filename, 510);
		if (filename_len > 0) {
			auto rpath = std::filesystem::relative(path, std::filesystem::path(filename));
			if (!rpath.empty()) {
				//output.emplace(DirEntry::Type::File, rpat);
			}
		}
	}
#endif
}

// ResourceManager

void ResourceManager::mount(std::string path, std::string native_path) {
	auto gpath = toGenericPath(path);
	if (!gpath.is_absolute()) {
		throw Error(Errc::InvalidParameter);
	}
	auto npath = std::filesystem::absolute(std::filesystem::canonical(native_path));
	std::unique_lock<std::mutex> locker(mutex_);
	mount_info_list_.emplace_front(gpath, npath.generic_string());
}

void ResourceManager::umount(std::string path) {
	std::filesystem::path gpath = toGenericPath(path);
	std::unique_lock<std::mutex> locker(mutex_);
	for (auto it = mount_info_list_.begin(); it != mount_info_list_.end(); ++it) {
		if (it->path() == gpath) {
			mount_info_list_.erase(it);
			return;
		}
	}
}

bool ResourceManager::exists(std::string path) const {
	auto gpath = toGenericPath(path);
	if (!gpath.is_absolute()) {
		throw Error(Errc::InvalidParameter);
	}
	std::unique_lock<std::mutex> locker(mutex_);
	for (const auto& mi: mount_info_list_) {
		auto rpath = std::filesystem::relative(gpath, mi.path());
		if (!rpath.empty()) {
			if (mi.exists(rpath)) {
				return true;
			}
		}
	}
	return false;
}

void ResourceManager::enumDir(std::string path, DirEntrySet& output) const {
	auto gpath = toGenericPath(path);
	if (!gpath.is_absolute()) {
		throw Error(Errc::InvalidParameter);
	}
	output.clear();
	std::unique_lock<std::mutex> locker(mutex_);
	for (auto it = mount_info_list_.rbegin(); it != mount_info_list_.rend(); ++it) {
		auto rpath = std::filesystem::relative(gpath, it->path());
		if (!rpath.empty()) {
			it->enumDir(rpath, output);
		}
	}
}

std::shared_ptr<Resource> ResourceManager::get(std::string path) {
	auto gpath = toGenericPath(path);
	if (!gpath.is_absolute()) {
		throw Error(Errc::InvalidParameter);
	}
	std::unique_lock<std::mutex> locker(mutex_);
	for (auto& mi : mount_info_list_) {
		auto rpath = std::filesystem::relative(gpath, mi.path());
		if (!rpath.empty()) {
			auto p = mi.get(rpath);
			if (p) {
				return p;
			}
		}
	}
	return nullptr;
}

VOLCANO_END
