//
//
#include <Volcano/Common.h>

VOLCANO_BEGIN

class PHYSFS_File_InputStream final : public sf::InputStream {
public:
	PHYSFS_File_InputStream(PHYSFS_File* fp, bool owned = true)
		: fp_(fp)
		, owned_(owned) {
		VOLCANO_ASSERT(fp_ != nullptr);
	}

	~PHYSFS_File_InputStream() {
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

std::unique_ptr<sf::InputStream> PHYSFS_openInputStream(const std::filesystem::path& filepath) {
	VOLCANO_ASSERT(!filepath.empty());
	VOLCANO_ASSERT(filepath.is_absolute());
	auto fp = PHYSFS_openRead(filepath.generic_string().c_str());
	if (fp == nullptr) {
		throw std::runtime_error(std::format("Failed to read physfs file: {}", filepath.generic_string()));
	}
	return std::make_unique<PHYSFS_File_InputStream>(fp);
}

ByteArray PHYSFS_readAll(const std::filesystem::path& filepath) {
	VOLCANO_ASSERT(!filepath.empty());
	VOLCANO_ASSERT(filepath.is_absolute());
	auto fp = PHYSFS_openInputStream(filepath);
	ByteArray data(fp->getSize().value_or(0));
	auto read_ret = fp->read(data.data(), data.size());
	if (!read_ret.has_value() || read_ret.value() != data.size()) {
		throw std::runtime_error(std::format("Failed to read physfs file: {}", filepath.generic_string()));
	}
	return data;
}

static std::string app_organization;
static std::string app_name;

const std::string& appOrganization() {
    return app_organization;
}

const std::string& appName() {
    return app_name;
}

void setAppInfo(const std::string& organization, const std::string& name) {
    assert(app_organization.empty());
    assert(app_name.empty());
    app_organization = organization;
    app_name = name;
}

VOLCANO_END
