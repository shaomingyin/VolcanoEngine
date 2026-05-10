//
//
#ifndef VOLCANO_PHYSFS_H
#define VOLCANO_PHYSFS_H

#include <physfs.h>
#include <async++.h>

#include <Volcano/Common.h>
#include <Volcano/ScopeGuard.h>

VOLCANO_BEGIN

namespace PhysFS {
	auto readFile(PHYSFS_File* file) {
		VOLCANO_ASSERT(file != nullptr);
		auto file_size = PHYSFS_fileLength(file);
		if (file_size < 0) {
			throw std::runtime_error("Failed to get physfs file size.");
		}
		ByteArray file_data(file_size);
		if (file_size > 0) {
			auto read_result = PHYSFS_readBytes(file, file_data.data(), file_size);
			if (read_result != file_size) {
				throw std::runtime_error("Failed to read physfs file.");
			}
		}
		return file_data;
	}

	auto readFile(const std::string& filepath) {
		VOLCANO_ASSERT(!filepath.empty());
		return readFile(PHYSFS_openRead(filepath.c_str()));
	}

	template <typename SCHEDULER>
	auto asyncReadFile(SCHEDULER& scheduler, PHYSFS_File* file) {
		VOLCANO_ASSERT(file != nullptr);
		return async::spawn(scheduler, [file] {
			return readFile(file);
		});
	}

	template <typename SCHEDULER>
	auto asyncReadFile(PHYSFS_File* file) {
		VOLCANO_ASSERT(file != nullptr);
		return asyncReadFile(async::default_scheduler(), file);
	}

	template <typename SCHEDULER>
	auto asyncReadFile(SCHEDULER& scheduler, const std::string& filepath) {
		VOLCANO_ASSERT(!filepath.empty());
		return async::spawn(scheduler, [filepath] {
			auto file = PHYSFS_openRead(filepath.c_str());
			if (file == nullptr) {
				throw std::runtime_error(std::format("Failed to open physfs file: {}", filepath));
			}
			auto file_guard = scopeGuard([file] {
				PHYSFS_close(file);
			});
			return readFile(file);
		});
	}

	template <typename SCHEDULER>
	auto asyncReadFile(const std::string& filepath) {
		VOLCANO_ASSERT(!filepath.empty());
		return asyncReadFile(async::default_scheduler(), filepath);
	}
} // namespace PhysFS

VOLCANO_END

#endif // VOLCANO_PHYSFS_H
