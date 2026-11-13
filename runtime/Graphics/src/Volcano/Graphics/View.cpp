#include <algorithm>
#include <cmath>
#include <functional>
#include <stdexcept>
#include <type_traits>

#include <Eigen/LU>

#include <Volcano/Math.h>
#include <Volcano/World/DirectionalLight.h>
#include <Volcano/World/Inherent.h>
#include <Volcano/World/Model.h>
#include <Volcano/World/OrthographicCamera.h>
#include <Volcano/World/Overlay.h>
#include <Volcano/World/PerspectiveCamera.h>
#include <Volcano/World/PointLight.h>
#include <Volcano/World/Scene.h>
#include <Volcano/World/SpotLight.h>
#include <Volcano/Graphics/Model.h>
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

bool View::visible(const World::Scene& scene, entt::entity entity) {
    const auto* inherent = scene.try_get<World::Inherent>(entity);
    return inherent == nullptr || (inherent->isEnabled() && inherent->isVisible());
}

Eigen::Affine3f View::entityTransform(const World::Scene& scene, entt::entity entity) {
    const auto* transform = scene.try_get<Transform>(entity);
    return transform != nullptr ? transform->affine() : Eigen::Affine3f::Identity();
}

const std::vector<View::PreparedItem>& View::opaqueItems() const {
    VOLCANO_ASSERT(ready());
    return opaque_;
}

const std::vector<View::PreparedItem>& View::transparentItems() const {
    VOLCANO_ASSERT(ready());
    return transparent_;
}

const std::vector<Light::Prepared>& View::preparedLights() const {
    VOLCANO_ASSERT(ready());
    return prepared_lights_;
}

void View::begin() noexcept {
    vg_tasks_.clear();
    overlays_.clear();
    state_ = State::Building;
    items_.clear();
    lights_.clear();
    opaque_.clear();
    transparent_.clear();
    prepared_lights_.clear();
}

void View::extract(const World::Scene& scene, entt::entity cameraEntity,
    entt::entity outputEntity,
    std::span<const std::reference_wrapper<const World::Canvas>> extraOverlays) {
    VOLCANO_ASSERT(scene.valid(cameraEntity));
    const auto* perspective = scene.try_get<World::PerspectiveCamera>(cameraEntity);
    const auto* orthographic = scene.try_get<World::OrthographicCamera>(cameraEntity);
    if ((perspective == nullptr) == (orthographic == nullptr)) {
        throw std::invalid_argument("Camera entity requires exactly one supported camera component");
    }
    const World::Camera& camera = perspective != nullptr
        ? static_cast<const World::Camera&>(*perspective)
        : *orthographic;
    const Eigen::Affine3f cameraTransform = entityTransform(scene, cameraEntity) * camera.offset().affine();
    if (!cameraTransform.matrix().allFinite() || !cameraTransform.linear().fullPivLu().isInvertible()) {
        throw std::invalid_argument("Camera transform must be finite and invertible");
    }

    begin();
    setProjection(camera.toMatrix());
    setViewMatrix(cameraTransform.inverse().matrix());

#if defined(VOLCANO_DEBUG) && !defined(NDEBUG)
    for (const auto entity : scene.view<World::Model>()) {
        if (visible(scene, entity)) {
            VOLCANO_ASSERT(scene.all_of<Model>(entity));
        }
    }
#endif

    for (const auto entity : scene.view<Model>()) {
        if (!visible(scene, entity)) {
            continue;
        }
        const auto& renderModel = scene.get<Model>(entity);
        const auto* modelComponent = scene.try_get<World::Model>(entity);
        VOLCANO_ASSERT(modelComponent == nullptr || renderModel.source() == modelComponent->source());
        const auto transform = entityTransform(scene, entity);
        const auto& parts = renderModel.parts();
        for (std::size_t partIndex = 0; partIndex < parts.size(); ++partIndex) {
            const auto& part = parts[partIndex];
            VOLCANO_ASSERT(part.mesh != nullptr);
            Material material = part.material;
            if (modelComponent != nullptr) {
                if (const auto* customMaterial = modelComponent->findMaterial(partIndex);
                    customMaterial != nullptr) {
                    const auto sourceEntity = customMaterial->texture(World::Material::Slot::Albedo);
                    if (sourceEntity != entt::null && sourceEntity != outputEntity && scene.valid(sourceEntity)) {
                        if (const auto* output = scene.try_get<Output>(sourceEntity);
                            output != nullptr && output->texture != nullptr) {
                            material.texture = output->texture;
                        }
                    }
                }
            }
            add(*part.mesh, material, transform * part.transform, part.transparent);
        }
    }

    auto addLights = [&]<typename Component>() {
        for (const auto entity : scene.view<Component>()) {
            if (!visible(scene, entity)) {
                continue;
            }
            const auto& source = scene.get<Component>(entity);
            const Eigen::Affine3f transform = entityTransform(scene, entity) * source.offset().affine();
            Light light;
            light.color = source.color();
            light.strength = source.strength();
            light.position = transform.translation();
            if constexpr (std::is_same_v<Component, World::PointLight>) {
                light.type = Light::Type::Point;
                light.range = source.range();
            } else {
                light.direction = transform.linear() * source.direction();
                if constexpr (std::is_same_v<Component, World::SpotLight>) {
                    light.type = Light::Type::Spot;
                    light.range = source.range();
                    light.innerAngle = source.innerAngle();
                    light.outerAngle = source.outerAngle();
                }
            }
            add(light);
        }
    };
    addLights.operator()<World::DirectionalLight>();
    addLights.operator()<World::PointLight>();
    addLights.operator()<World::SpotLight>();

    if (outputEntity == entt::null) {
        for (const auto entity : scene.view<World::Overlay, World::Canvas>()) {
            if (!visible(scene, entity)) {
                continue;
            }
            const auto& canvas = scene.get<World::Canvas>(entity);
            if (canvas.ready()) {
                add(canvas);
            }
        }
        for (const auto& overlay : extraOverlays) {
            if (overlay.get().ready()) {
                add(overlay.get());
            }
        }
    }

    end();
}

void View::add(const Mesh& mesh, const Material& material, const Transform& transform, bool transparent) {
    VOLCANO_ASSERT(state_ == State::Building);
    add(mesh, material, transform.affine(), transparent);
}

void View::add(const Mesh& mesh, const Material& material, const Eigen::Affine3f& transform, bool transparent) {
    VOLCANO_ASSERT(state_ == State::Building);
    items_.push_back({mesh, material, transform, transparent});
}

void View::add(const Light& light) {
    VOLCANO_ASSERT(state_ == State::Building);
    lights_.push_back(light);
}

void View::end() {
    VOLCANO_ASSERT(state_ == State::Building);
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

void View::add(Target& target, const World::Canvas& commands) {
    VOLCANO_ASSERT(state_ == State::Building && commands.ready());
    vg_tasks_.push_back({target, commands});
}

void View::add(const World::Canvas& commands) {
    VOLCANO_ASSERT(state_ == State::Building && commands.ready());
    overlays_.push_back(commands);
}

VOLCANO_GRAPHICS_END
