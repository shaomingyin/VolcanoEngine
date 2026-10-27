#include <algorithm>
#include <cmath>
#include <functional>
#include <stdexcept>

#include <Volcano/Graphics/View.h>
#include <Volcano/Math.h>

VOLCANO_GRAPHICS_BEGIN

void View::begin() noexcept {
    state_ = State::Building;
    items_.clear();
    lights_.clear();
    opaque_.clear();
    transparent_.clear();
    prepared_lights_.clear();
}

void View::requireBuilding() const {
    if (state_ != State::Building) {
        throw std::logic_error("View submission requires begin() before adding data or calling end()");
    }
}

void View::requireReady() const {
    if (!ready()) {
        throw std::logic_error("View rendering requires a successful begin()/end() submission");
    }
}

void View::addItem(const Mesh& mesh, const Material& material, const Transform& transform, bool transparent) {
    requireBuilding();
    addItem(mesh, material, transform.affine(), transparent);
}

void View::addItem(const Mesh& mesh, const Material& material, const Eigen::Affine3f& transform, bool transparent) {
    requireBuilding();
    items_.push_back({mesh, material, transform, transparent});
}

void View::addLight(const Light& light) {
    requireBuilding();
    lights_.push_back(light);
}

void View::end() {
    requireBuilding();
    opaque_.clear();
    transparent_.clear();
    prepared_lights_.clear();

    if (!ambient_light_.allFinite() || (ambient_light_.array() < 0.0f).any()) {
        throw std::invalid_argument("Ambient light must be finite and nonnegative");
    }
    if (!projection_.allFinite() || !view_.allFinite()) {
        throw std::invalid_argument("Camera matrices must be finite");
    }

    prepared_lights_.reserve(lights_.size());
    for (const auto& light : lights_) {
        prepared_lights_.emplace_back(light);
    }

    const auto transparent_count = std::ranges::count_if(items_, &DrawItem::transparent);
    transparent_.reserve(static_cast<std::size_t>(transparent_count));
    opaque_.reserve(items_.size() - static_cast<std::size_t>(transparent_count));
    for (std::size_t index = 0; index < items_.size(); ++index) {
        const auto& item = items_[index];
        const auto& model = item.transform;
        if (!model.matrix().allFinite() ||
            !model.matrix().row(3).isApprox(Eigen::RowVector4f(0, 0, 0, 1))) {
            throw std::invalid_argument("Model transform must be finite and affine");
        }
        Eigen::Matrix3f normal = Eigen::Matrix3f::Identity();
        if (item.material.lit) {
            normal = Eigen::normalMatrix(model);
        }

        float depth = 0;
        if (item.transparent) {
            depth = (view_ * model.translation().homogeneous()).z();
            if (!std::isfinite(depth)) {
                throw std::invalid_argument("Transparent item depth must be finite");
            }
        }

        auto& items = item.transparent ? transparent_ : opaque_;
        items.push_back({index, model, normal, depth});
    }

    std::stable_sort(opaque_.begin(), opaque_.end(), [this](const auto& left, const auto& right) {
        const auto& left_item = items_[left.index];
        const auto& right_item = items_[right.index];
        if (left_item.material.texture != right_item.material.texture) {
            return std::less<const Texture*>{}(left_item.material.texture, right_item.material.texture);
        }
        if (left_item.material.lit != right_item.material.lit) {
            return left_item.material.lit < right_item.material.lit;
        }

        return std::less<const Mesh*>{}(&left_item.mesh.get(), &right_item.mesh.get());
    });

    // shortcut: sort by model origin, split intersecting meshes if this order is insufficient.
    std::stable_sort(transparent_.begin(), transparent_.end(), [](const auto& left, const auto& right) {
        return left.depth < right.depth;
    });
    state_ = State::Ready;
}

VOLCANO_GRAPHICS_END
