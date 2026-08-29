//
//
#include <utility>

#include <Volcano/World/Model.h>

VOLCANO_WORLD_BEGIN

Model::Model() noexcept {
}

const std::vector<Model::Part>& Model::parts() const noexcept {
    return parts_;
}

std::vector<Model::Part>& Model::parts() noexcept {
    return parts_;
}

void Model::add(const Part& part) {
    parts_.push_back(part);
}

void Model::add(Part&& part) {
    parts_.push_back(std::move(part));
}

bool Model::empty() const noexcept {
    return parts_.empty();
}

VOLCANO_WORLD_END
