//
//
#ifndef VOLCANO_RESOURCEMANAGER_H
#define VOLCANO_RESOURCEMANAGER_H

#include <set>
#include <list>
#include <memory>
#include <string>
#include <variant>
#include <filesystem>
#include <mutex>

#include <miniz.h>

#include <Volcano/Common.h>
#include <Volcano/Cache.h>

VOLCANO_BEGIN

class Resource {
public:
	Resource() = default;
	virtual ~Resource() = default;

public:
	virtual int64_t size() const = 0;
	virtual bool load(void* buf, int64_t buf_size) = 0;
};

class ResourceManager {
public:
	struct DirEntry final {
		enum class Type {
			Directory = 0,
			File,
			Unknown
		};

		Type type;
		std::string name;

		DirEntry(Type t, std::string n)
			: type(t)
			, name(std::move(n)) {
		}

		bool operator<(const DirEntry& other) const {
			return (name < other.name);
		}
	};

	using DirEntrySet = std::set<DirEntry>;

	class MountInfo final {
	public:
		MountInfo(const std::filesystem::path& path, const std::filesystem::path& native_path);
		~MountInfo();

	public:
		const std::filesystem::path& path() const {
			return path_;
		}

		const std::filesystem::path& nativePath() const {
			return native_path_;
		}

		bool exists(const std::filesystem::path& path) const;
		void enumDir(const std::filesystem::path& path, DirEntrySet& output) const;
		std::shared_ptr<Resource> get(const std::filesystem::path& path);

	private:
		void enumNativeDir(const std::filesystem::path& path, DirEntrySet& output) const;
		void enumZipDir(const std::filesystem::path& path, DirEntrySet& output) const;

	private:
		using Native = std::variant<std::filesystem::path, std::shared_ptr<mz_zip_archive>>;

		std::mutex mutex_;
		std::filesystem::path path_;
		std::filesystem::path native_path_;
		Native native_;
	};

	using MountInfoList = std::list<MountInfo>;

public:
	ResourceManager() = default;
	virtual ~ResourceManager() = default;

public:
	const MountInfoList& mountInfoList() const {
		return mount_info_list_;
	}

	void mount(const std::filesystem::path& path, const std::filesystem::path& native_path);
	void umount(const std::filesystem::path& path);
	bool exists(const std::filesystem::path& path) const;
	void enumDir(const std::filesystem::path& path, DirEntrySet& output) const;
	std::shared_ptr<Resource> get(const std::filesystem::path& path);

	void update();

protected:
	static std::filesystem::path toGenericPath(const std::filesystem::path& path) {
		auto tmp1 = path.lexically_normal();
		auto tmp2 = tmp1.generic_string();
		return std::filesystem::path(tmp2);
	}

	static bool isAbsolute(const std::filesystem::path path) {
		if (path.is_absolute()) {
			return true;
		}
		if (!path.empty() && *(path.begin()) == "/") {
			return true;
		}
		return false;
	}

private:
	using ResourceCache = Cache<std::filesystem::path, std::shared_ptr<Resource>>;

private:
	mutable std::mutex mutex_;
	MountInfoList mount_info_list_;
	ResourceCache resource_cache_;
};

VOLCANO_END

#endif // VOLCANO_RESOURCEMANAGER_H
