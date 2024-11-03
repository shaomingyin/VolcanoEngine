//
//
#include <Volcano/Error.h>
#include <Volcano/System/Loader.h>

VOLCANO_SYSTEM_BEGIN

Loader::Loader(World::Scene& scene)
	: scene_(scene)
	, state_(State::Idle)
	, progress_(0) {
}

Loader::~Loader() {
}

void Loader::start() {
	if (state_ != State::Idle) {
		throw Error(Errc::InvalidState);
	}

	state_ = State::Loading;
	progress_ = 0;

	tf::Taskflow taskflow;
	taskflow.emplace([this](tf::Subflow& subflow) mutable {
		loadManifest(subflow);
	});

	load_future_ = executor_.run(std::move(taskflow));
}

void Loader::loadManifest(tf::Subflow& subflow) {
	progress_text_ = "Loading manifest...";
	PHYSFS_File* manifest_fp = PHYSFS_openRead("/Manifest.json");
	if (manifest_fp == nullptr) {
		throw Error(Errc::NotExisted);
	}
	auto manifest_sz = PHYSFS_fileLength(manifest_fp);
	if (manifest_sz > 0) {
		Buffer manifest_data(manifest_sz);
		int ret = PHYSFS_readBytes(manifest_fp, manifest_data.data(), manifest_sz);
		if (ret == manifest_sz) {
			auto manifest = nlohmann::json::parse(manifest_data);

		}
	}
}

VOLCANO_SYSTEM_END
